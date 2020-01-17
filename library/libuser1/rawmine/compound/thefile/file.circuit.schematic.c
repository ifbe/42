#include "libuser.h"
int parsefv(float* vec, int flen, u8* str, int slen);




struct wireindex{
	int off;
	int cnt;
	int val;
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
		if(0xa > buf[j])break;
		if(0xa== buf[j]){
			sts[ioff].cnt = parsewiring_oneline(buf+k, j-k, dat,foff);
			sts[ioff].val = 0;
			sts[ioff].off = foff;
			//say("%d,%d\n", sts[ioff].off, sts[ioff].cnt);

			foff += sts[ioff].cnt;
			ioff += 1;
			k = j+1;
		}
	}
	sts[ioff].cnt = 0;
	return j;
}




u32 sch_color(int val)
{
	if(val < 0)return 0x0000ff;
	if(val > 0)return 0xff0000;
	return 0xffffff;
}
void broadcast_except(struct halfrel* self, void* except, void* stack, int rsp, u8* buf, int len)
{
	struct entity* ent;
	struct relation* rel;
	if(rsp>3)return;

	ent = self->pchip;
say("rsp=%d\n",rsp);

	rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if((rel->srcflag == self->flag) && (rel->dst != except)){
			entitywrite((void*)(rel->dst), (void*)(rel->src), stack, rsp, buf, len);
		}
		rel = samesrcnextdst(rel);
	}

	rel = ent->irel0;
	while(1){
		if(0 == rel)break;
		if((rel->dstflag == self->flag)&&(rel->src != except)){
			entitywrite((void*)(rel->src), (void*)(rel->dst), stack, rsp, buf, len);
		}
		rel = samedstnextsrc(rel);
	}
}




void sch_simple(struct entity* ent, struct wireindex* sts)
{
	u8 any = 0;
	u8 positive = 'p';
	u8 negative = 'n';

	if(sts[0].val <= 0){
		sts[0].val = 1;
		relationwrite(ent, 'a', 0, 0, &positive, 1);
	}
	else{
		sts[0].val = -1;
		relationwrite(ent, 'a', 0, 0, &negative, 1);
	}
}
void sch_complex(struct entity* ent, struct wireindex* sts, u8* buf, int len)
{
	int j;
	u8 any = 0;
	u8 positive = 'p';
	u8 negative = 'n';

	//step0: clear
	for(j=0;j<16;j++){
		if(0 == sts[j].cnt)break;
		sts[j].val = 0;
	}
	relationwrite(ent, 'a', 0, 0, &any, 0);
	relationwrite(ent, 'b', 0, 0, &any, 0);
	relationwrite(ent, 'c', 0, 0, &any, 0);
	relationwrite(ent, 'd', 0, 0, &any, 0);

	//step1: send
	switch(buf[0]){
	case '0':{
		sts[0].val = -1;
		relationwrite(ent, 'a', 0, 0, &negative, 0);
		sts[1].val = -1;
		relationwrite(ent, 'b', 0, 0, &negative, 0);
		break;
	}
	case '1':{
		sts[0].val = 1;
		relationwrite(ent, 'a', 0, 0, &positive, 0);
		sts[1].val = -1;
		relationwrite(ent, 'b', 0, 0, &negative, 0);
		break;
	}
	case '2':{
		sts[0].val = -1;
		relationwrite(ent, 'a', 0, 0, &negative, 0);
		sts[1].val = 1;
		relationwrite(ent, 'b', 0, 0, &positive, 0);
		break;
	}
	case '3':{
		sts[0].val = 1;
		relationwrite(ent, 'a', 0, 0, &positive, 0);
		sts[1].val = 1;
		relationwrite(ent, 'b', 0, 0, &positive, 0);
		break;
	}
	}

	//step2: read
	u8 val;
	int k,err=0;
	for(j=0;j<16;j++){
		if(0 == sts[j].cnt)break;
		if(0 == sts[j].val)err++;
	}
	while(1){
		k = 0;
		for(j=1;j<16;j++){
			if(0 == sts[j].cnt)break;
			if(0 != sts[j].val)continue;

			val = 0;
			relationread(ent, 'a'+j, 0, 0, &val, 1);
			if(0 == val)k += 1;
			else if('p' == val){
				sts[j].val = 1;
				relationwrite(ent, 'a'+j, 0, 0, &positive, 1);
			}
			else if('n' == val){
				sts[j].val =-1;
				relationwrite(ent, 'a'+j, 0, 0, &negative, 1);
			}
		}
		say("k=%d,err=%d\n", k,err);
		if(k == 0)break;	//all done
		if(k >= err)break;	//can not
		err = k;
	}
}




static void sch_draw_gl41(
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

	int j,k;
	int off,cnt,rgb;
	vec3 ta,tb;
	for(k=0;k<0x10000/sizeof(struct wireindex);k++){
		cnt = sts[k].cnt;
		if(0 == cnt)break;

		off = sts[k].off;
		rgb = sch_color(sts[k].val);
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
	}
}
void sch_read_board(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> sch
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	act = self->pchip;slot = self->pfoot;
	win = peer->pchip;geom = peer->pfoot;
	wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
	wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
	sch_draw_gl41(act,slot, win,geom, wnd,area);
}
int sch_read_child(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* scene;
	struct relation* rel;
	//say("@sch_read\n");

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
int sch_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	if(stack && ('v' == len)){
		sch_read_child(self,peer, stack,rsp, buf,len);
		sch_read_board(self,peer, stack,rsp, buf,len);
	}
	return 0;
}
int sch_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent = self->pchip;
	if(0 == ent)return 0;
	struct wireindex* sts = ent->buf0;
	if(0 == sts)return 0;
say("sch_write: %.4s\n", &self->flag);
	switch(self->flag){
		case _ev_:{
			if('a' == buf[0])sch_simple(ent, sts);
			else sch_complex(ent, sts, buf, len);
			break;
		}
		case 'b':{
			if('p' == buf[0])sts[1].val = 1;
			if('n' == buf[0])sts[1].val =-1;
			broadcast_except(self, peer, stack, rsp+1, buf, 1);
			break;
		}
		case 'c':{
			if('p' == buf[0])sts[2].val = 1;
			if('n' == buf[0])sts[2].val =-1;
			broadcast_except(self, peer, stack, rsp+1, buf, 1);
			break;
		}
		case 'd':{
			if('p' == buf[0])sts[3].val = 1;
			if('n' == buf[0])sts[3].val =-1;
			broadcast_except(self, peer, stack, rsp+1, buf, 1);
			break;
		}
	}
	return 0;
}
int sch_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int sch_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@sch_linkup: %.4s\n", &self->flag);
	return 0;
}




int sch_search(struct entity* scene)
{
	return 0;
}
int sch_modify(struct entity* scene)
{
	return 0;
}
int sch_delete(struct entity* scene)
{
	return 0;
}
int sch_create(struct entity* scene, void* arg, int argc, u8** argv)
{
	int ret;
	say("@sch_create\n");
	if(0 == arg)return 0;

	scene->buf0 = memorycreate(0x10000, 0);
	ret = openreadclose(arg, 0, scene->buf0, 0x10000);
	if(ret <= 0)return 0;

	scene->buf1 = memorycreate(0x10000, 0);
	parsewiring(scene->buf0, scene->buf1);
	return 0;
}
