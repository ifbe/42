#include "libuser.h"
int parsefv(float* vec, int flen, u8* str, int slen);
int relation_readall( void* item, int foot, void* arg, int idx, void* buf, int len);
int relation_writeall( void* item, int foot, void* arg, int idx, void* buf, int len);




struct wireindex{
	int off;
	int cnt;
	float volt;
	float grad;
	u8 sure;
	u8 temp;
};
static int parsewiring_oneline(u8* buf, int len, float* dat, int cnt)
{
	//printmemory(buf,len);
	//say("%llx,%llx\n",dat,sts);
	int j,k,ret=0;
	float* cur;
	for(j=0;j<len;j++){
		if('(' == buf[j])k = j+1;
		if(')' == buf[j]){
			cur = &dat[6*(cnt+ret)];
			parsefv(cur, 6, buf+k,j-k);
			//say("%f,%f,%f,%f,%f,%f\n",cur[0],cur[1],cur[2],cur[3],cur[4],cur[5]);
			ret++;
		}
	}
	//say("ret=%d\n",ret);
	return ret;
}
static int parsewiring(u8* buf, float* dat)
{
	//printmemory(str, 0x100);
	int j,k=0;
	int ioff=0,foff=0;
	struct wireindex* sts = (void*)buf;
	for(j=0;j<0x10000;j++){
		if(buf[j] <= 0xa){
			sts[ioff].cnt = parsewiring_oneline(buf+k, j-k, dat,foff);
			sts[ioff].off = foff;
			sts[ioff].volt = 0.0;
			sts[ioff].grad = 0.0;
			sts[ioff].sure = 1;
			sts[ioff].temp = 0;
			//say("%d,%d\n", sts[ioff].off, sts[ioff].cnt);

			if(buf[j] < 0xa)break;
			foff += sts[ioff].cnt;
			ioff += 1;
			k = j+1;
		}
	}
	sts[ioff].cnt = 0;
	return j;
}




static void analog_voltage(struct wireindex* dst, float* volt)
{
	dst->volt = *volt;
	dst->grad = 0.0;

	dst->sure = 1;
}
static void analog_emulate(struct entity* ent, struct wireindex* sts, u8* buf, int len)
{
	//clear, ensure
	int j,k;
	for(j=1;j<16;j++){
		if(0 == sts[j].cnt)break;
		sts[j].sure = 0;
		sts[j].volt = 0.0;
	}
	sts[0].sure = 1;
	sts[0].volt = (float)(buf[0]-0x30);
	relation_writeall(ent, 'a', 0, 0, &sts[0], 0);
}
static void analog_decent_V(struct entity* ent, struct wireindex* sts)
{
	int j;
	for(j=1;j<16;j++){
		if(0 == sts[j].cnt)break;
		if(0 != sts[j].sure)continue;

		sts[j].grad = 0.0;
		relation_readall(ent, 'a'+j, 0, 'V', sts, j);
	}
	for(j=1;j<16;j++){
		if(0 == sts[j].cnt)break;
		if(0 != sts[j].sure)continue;

		sts[j].volt -= sts[j].grad;
		say("%f\n",sts[j].volt);
	}
}
static void analog_decent_R(struct entity* ent, struct wireindex* sts)
{
	int j;
	for(j=1;j<16;j++){
		if(0 == sts[j].cnt)break;
		if(0 != sts[j].sure)continue;

		sts[j].grad = 0.0;
		relation_readall(ent, 'a'+j, 0, 'R', sts, j);
	}
	for(j=1;j<16;j++){
		if(0 == sts[j].cnt)break;
		if(0 != sts[j].sure)continue;

		sts[j].volt -= sts[j].grad;
		say("%f\n",sts[j].volt);
	}
}




static void analog_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41solid_rect(wnd, 0x404040, vc, vr, vf);

	struct wireindex* sts = act->buf0;
	if(0 == sts)return;
	float* dat = act->buf1;
	if(0 == dat)return;

	analog_decent_R(act, sts);
	analog_decent_V(act, sts);

	int j,k;
	int off,cnt,rgb;
	vec3 ta,tb;
	vec3 tc,tr,tf;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/16;
		tf[j] = vf[j]/16;
	}
	for(k=0;k<0x10000/sizeof(struct wireindex);k++){
		cnt = sts[k].cnt;
		if(0 == cnt)break;

		off = sts[k].off;
		rgb = 0xffffff;
		for(j=6*off;j<6*(off+cnt);j+=6){
			//say("j=%d\n",j);
			ta[0] = vc[0]+dat[j+0];
			ta[1] = vc[1]+dat[j+1];
			ta[2] = vc[2]+dat[j+2];
			tb[0] = vc[0]+dat[j+3];
			tb[1] = vc[1]+dat[j+4];
			tb[2] = vc[2]+dat[j+5];
			gl41line(wnd, rgb, ta, tb);
		}
		for(j=0;j<3;j++)tc[j] = vc[j]+vt[j]+ (dat[6*off+j]+dat[6*off+j+3])/2;
		carvefloat(wnd, rgb, tc,tr,tf, sts[k].volt);
	}
}
void analog_read_board(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> analog
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	act = self->pchip;slot = self->pfoot;
	win = peer->pchip;geom = peer->pfoot;
	wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
	wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
	analog_draw_gl41(act,slot, win,geom, wnd,area);
}
int analog_read_child(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* scene;
	struct relation* rel;
	//say("@analog_read\n");

	scene = self->pchip;
	if(0 == scene)return 0;
	rel = scene->orel0;
	if(0 == rel)return 0;

	while(1){
		if(0 == rel)break;
		if(rel->srcfoot)entityread((void*)(rel->dst), (void*)(rel->src), stack, rsp, buf, len);
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int analog_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	if(stack && ('v' == len)){
		analog_read_child(self,peer, stack,rsp, buf,len);
		analog_read_board(self,peer, stack,rsp, buf,len);
	}
	return 0;
}
int analog_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* ent = self->pchip;
	if(0 == ent)return 0;
	struct wireindex* sts = ent->buf0;
	if(0 == sts)return 0;
say("analog_write: %.4s\n", &self->flag);
	switch(self->flag){
		case _ev_:analog_emulate(ent,sts, buf,len);break;
		case 'b': analog_voltage(&sts[1], buf);break;
		case 'c': analog_voltage(&sts[2], buf);break;
		case 'd': analog_voltage(&sts[3], buf);break;
	}
	return 0;
}
int analog_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int analog_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@analog_linkup: %.4s\n", &self->flag);
	return 0;
}




int analog_search(struct entity* scene)
{
	return 0;
}
int analog_modify(struct entity* scene)
{
	return 0;
}
int analog_delete(struct entity* scene)
{
	return 0;
}
int analog_create(struct entity* scene, void* arg, int argc, u8** argv)
{
	int ret;
	say("@analog_create\n");
	if(0 == arg)return 0;

	scene->buf0 = memorycreate(0x10000, 0);
	ret = openreadclose(arg, 0, scene->buf0, 0x10000);
	if(ret <= 0)return 0;

	scene->buf1 = memorycreate(0x10000, 0);
	parsewiring(scene->buf0, scene->buf1);
	return 0;
}
