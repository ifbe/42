#include "libuser.h"
int parsefv(float* vec, int flen, u8* str, int slen);




struct joint{
	float here[3];
	int exist;

	float grad[3];
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
	parsefv(jo->here, 3, buf+j, 16);
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
				jo[ioff].grad[0] = 0.0;
				jo[ioff].grad[1] = 0.0;
				jo[ioff].grad[2] = 0.0;
				say("%d: %f,%f,%f\n", ioff, jo[ioff].here[0], jo[ioff].here[1], jo[ioff].here[2]);
				ioff += 1;
			}
			if(buf[j] < 0xa)break;
			tmp = j+1;
		}
	}

	jo[ioff].exist = 0;
	return j;
}




static void force_decent_spring(_obj* ent, struct joint* jo, _syn* stack,int sp)
{
	int j;
	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].grad[0] = 0.0;
		jo[j].grad[1] = 0.0;
		jo[j].grad[2] = 0.0;
		take_data_from_them(ent,'a'+j, stack,sp, 0,'R', jo,j);
	}

	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].here[0] -= jo[j].grad[0];
		jo[j].here[1] -= jo[j].grad[1];
		jo[j].here[2] -= jo[j].grad[2];
		say("@force_decent_spring: %f,%f,%f\n",jo[j].here[0], jo[j].here[1], jo[j].here[2]);
	}
}
static void force_decent_stick(_obj* ent, struct joint* jo, _syn* stack,int sp)
{
	int j;
	for(j=1;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].grad[0] = 0.0;
		jo[j].grad[1] = 0.0;
		jo[j].grad[2] = 0.0;
		take_data_from_them(ent,'a'+j, stack,sp, 0,'V', jo,j);
	}
	for(j=1;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].here[0] -= jo[j].grad[0];
		jo[j].here[1] -= jo[j].grad[1];
		jo[j].here[2] -= jo[j].grad[2];
		say("force_decent_stick:%f,%f,%f\n",jo[j].here[0], jo[j].here[1], jo[j].here[2]);
	}
}




static void force_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int j;
	vec3 tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(wnd, 0x404040, vc, vr, vf, vt);


	struct joint* jo = act->listptr.buf0;
	if(0 == jo[0].exist)return;

	for(j=0;j<3;j++){tr[j] = tf[j] = tu[j] = 0.0;}
	tr[0] = tf[1] = tu[2] = 10.0;
	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		gl41solid_sphere(wnd, 0x808080, jo[j].here,tr,tf,tu);
	}
	tr[0] = tf[1] = tu[2] = 100.0;
	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		gl41ascii_center(wnd, 0xff0000, jo[j].here,tr,tu,'a'+j);
	}
}
void force_read_board(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;
	if('v' != key)return;

	//joint
	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	force_draw_gl41(ent,slot, wor,geom, wnd,area);
}
void force_read_inner(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key)
{
	//spring, stick
	struct joint* jo = ent->listptr.buf0;
	if(0 == jo[0].exist)return;

	void** tab = ent->listptr.buf1;
	if(0 == tab)return;

	int j;
	for(j=0;j<16;j++){
		if(0 == tab[j])break;
		say("node: %llx\n", tab[j]);

		stack[sp+0].pchip = ent;
		stack[sp+0].flag = 0;
		stack[sp+1].pchip = tab[j];
		stack[sp+1].flag = 'f';
		entity_take(tab[j],0, stack,sp+2, 0,0, jo,0);
	}
}
int force_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct joint* jo = ent->listptr.buf0;
	if(0 == stack)return 0;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//slot type known: do work based on slot type
	//switch(stack[sp-1].flag){
	//}

	//slot type unknown: do work based on caller fmt
	switch(caller->hfmt){
	case _gl41list_:
		break;
	default:
		if(jo[0].exist){
			force_decent_spring(ent,jo, stack,sp);
			force_decent_stick(ent,jo, stack,sp);
		}
		force_read_board(ent,foot, stack,sp, arg,key);
		force_read_inner(ent,foot, stack,sp, arg,key);
		break;
	}
	return 0;
}
int force_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
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

	_obj* ent = self->pchip;
	if(0 == ent)return 0;

	void** tab = ent->listptr.buf1;
	if(0 == tab)return 0;

	for(j=0;j<16;j++){
		if(tab[j] == peer->pchip)break;
		if(0 == tab[j]){tab[j] = peer->pchip;break;}
	}
	return 0;
}




int force_search(_obj* scene)
{
	return 0;
}
int force_modify(_obj* scene)
{
	return 0;
}
int force_delete(_obj* scene)
{
	return 0;
}
int force_create(_obj* scene, void* arg, int argc, u8** argv)
{
	int ret;
	void* buf;
	say("@force_create\n");
	if(0 == arg)return 0;

	scene->listptr.buf0 = memorycreate(0x10000, 0);
	scene->listptr.buf1 = memorycreate(0x10000, 0);

	buf = scene->listptr.buf0;
	if(0 == buf)return 0;

	ret = openreadclose(arg, 0, buf+0x8000, 0x8000);
	if(ret <= 0)return 0;

	parsejoint(buf, buf+0x8000);
	return 0;
}
