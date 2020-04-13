#include "libuser.h"
int parsefv(float* vec, int flen, u8* str, int slen);
int relation_readall( void* item,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len);
int relation_writeall(void* item,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len);




struct joint{
	float x;
	float y;
	float z;
	int exist;

	float gradx;
	float grady;
	float gradz;
	int sure;
};
static int parsejoint_oneline(u8* buf, struct joint* jo)
{
	int j;
	if(buf[0] < '0')return 0;
	if(buf[0] > '1')return 0;
	jo->sure = buf[0] - '0';

	for(j=1;j<8;j++){
		if('-' == buf[j])goto found;
		if((buf[j] >= '0')&&(buf[j] <= '9'))goto found;
	}
	return 0;

found:
	parsefv(&jo->x, 3, buf+j, 16);
	jo->exist = 1;
	return 1;
}
static int parsejoint(struct joint* jo, u8* buf)
{
	int j,k;
	int tmp=0,ioff=0;
	for(j=0;j<0x8000;j++){
		if(buf[j] <= 0xa){
			if(j-k<4)break;
			k = parsejoint_oneline(buf+tmp, &jo[ioff]);
			if(k){
				jo[ioff].gradx = 0.0;
				jo[ioff].grady = 0.0;
				jo[ioff].gradz = 0.0;
				say("%d: %f,%f,%f\n", ioff, jo[ioff].x, jo[ioff].y, jo[ioff].z);
				ioff += 1;
			}
			if(buf[j] < 0xa)break;
			tmp = j+1;
		}
	}

	jo[ioff].exist = 0;
	return j;
}




static void force_decent_spring(struct entity* ent, struct joint* jo, _syn* stack,int sp)
{
	int j;
	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].gradx = 0.0;
		jo[j].grady = 0.0;
		jo[j].gradz = 0.0;
		relation_readall(ent,'a'+j, stack,sp, 0,'R', jo,j);
	}

	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].x -= jo[j].gradx;
		jo[j].y -= jo[j].grady;
		jo[j].z -= jo[j].gradz;
		say("@force_decent_spring: %f,%f,%f\n",jo[j].x, jo[j].y, jo[j].z);
	}
}
static void force_decent_stick(struct entity* ent, struct joint* jo, _syn* stack,int sp)
{
	int j;
	for(j=1;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].gradx = 0.0;
		jo[j].grady = 0.0;
		jo[j].gradz = 0.0;
		relation_readall(ent,'a'+j, stack,sp, 0,'V', jo,j);
	}
	for(j=1;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].x -= jo[j].gradx;
		jo[j].y -= jo[j].grady;
		jo[j].z -= jo[j].gradz;
		say("force_decent_stick:%f,%f,%f\n",jo[j].x, jo[j].y, jo[j].z);
	}
}




static void force_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_prism4(wnd, 0x404040, vc, vr, vf, vt);


	struct joint* jo = act->buf0;
	if(0 == jo[0].exist)return;

	for(j=0;j<3;j++){tr[j] = tf[j] = tu[j] = 0.0;}
	tr[0] = tf[1] = tu[2] = 10.0;
	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		gl41solid_sphere(wnd, 0x808080, &jo[j].x,tr,tf,tu);
	}
	tr[0] = tf[1] = tu[2] = 100.0;
	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		carveascii_center(wnd, 0xff0000, &jo[j].x,tr,tu,'a'+j);
	}
}
void force_read_board(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(0 == stack)return;
	if('v' != key)return;

	//joint
	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	force_draw_gl41(ent,slot, wor,geom, wnd,area);
}
void force_read_inner(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	//spring, stick
	struct joint* jo = ent->buf0;
	if(0 == jo[0].exist)return;

	void** tab = ent->buf1;
	if(0 == tab)return;

	int j;
	for(j=0;j<16;j++){
		if(0 == tab[j])break;
		say("node: %llx\n", tab[j]);

		stack[sp+0].pchip = ent;
		stack[sp+0].flag = 0;
		stack[sp+1].pchip = tab[j];
		stack[sp+1].flag = 'f';
		entityread(tab[j],'f', stack,sp+2, 0,0, jo,0);
	}
}
int force_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(stack && ('v' == key)){
		struct joint* jo = ent->buf0;
		if(jo[0].exist){
			force_decent_spring(ent,jo, stack,sp);
			force_decent_stick(ent,jo, stack,sp);
		}
		force_read_board(ent,foot, stack,sp, arg,key);
		force_read_inner(ent,foot, stack,sp, arg,key);
	}
	return 0;
}
int force_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int force_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int force_linkup(struct halfrel* self, struct halfrel* peer)
{
	int j;
	if(0 == self)return 0;
	say("@force_linkup: %.4s\n", &self->flag);

	j = self->flag;
	if('a' > j)return 0;
	if('z' < j)return 0;

	struct entity* ent = self->pchip;
	if(0 == ent)return 0;

	void** tab = ent->buf1;
	if(0 == tab)return 0;

	for(j=0;j<16;j++){
		if(tab[j] == peer->pchip)break;
		if(0 == tab[j]){tab[j] = peer->pchip;break;}
	}
	return 0;
}




int force_search(struct entity* scene)
{
	return 0;
}
int force_modify(struct entity* scene)
{
	return 0;
}
int force_delete(struct entity* scene)
{
	return 0;
}
int force_create(struct entity* scene, void* arg, int argc, u8** argv)
{
	int ret;
	void* buf;
	say("@force_create\n");
	if(0 == arg)return 0;

	scene->buf0 = memorycreate(0x10000, 0);
	scene->buf1 = memorycreate(0x10000, 0);

	buf = scene->buf0;
	if(0 == buf)return 0;

	ret = openreadclose(arg, 0, buf+0x8000, 0x8000);
	if(ret <= 0)return 0;

	parsejoint(buf, buf+0x8000);
	return 0;
}
