#include "libuser.h"
#define PERPIN buf0
#define VERTEX buf1
#define STAMP data3
int parsefv(float* vec, int flen, u8* str, int slen);
int relation_readall(void*,int, void*,int, void*,int, void*,int);




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



static int digital_recur(_syn* stack,int sp, struct entity* ent)
{
	int j;
	for(j=0;j<sp;j++){
		if(ent == stack[j].pchip)return 1;
	}
	return 0;
}
static void digital_broadcast(struct entity* ent, int pin, _syn* stack,int sp, u8* buf, int len)
{
	struct relation* rel;
	struct entity* chip;
	int foot;

	rel = ent->irel0;
	while(1){
		if(0 == rel)break;
		if(pin != rel->dstflag)goto next1;

		chip = rel->psrcchip;
		if(digital_recur(stack,sp, chip))goto next1;

		stack[sp+0].pchip = ent;
		stack[sp+1].pchip = chip;

		foot = rel->srcflag;
		entitywrite(chip,foot, stack,sp+2, 0,ent->STAMP, buf,len);
next1:
		rel = samedstnextsrc(rel);
	}

	rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(pin != rel->srcflag)goto next2;

		chip = rel->pdstchip;
		if(digital_recur(stack,sp, chip))goto next2;

		stack[sp+0].pchip = ent;
		stack[sp+1].pchip = chip;

		foot = rel->dstflag;
		entitywrite(chip,foot, stack,sp+2, 0,ent->STAMP, buf,len);
next2:
		rel = samesrcnextdst(rel);
	}
}
void digital_complex(struct entity* ent,struct wireindex* sts, _syn* stack,int sp, u8* buf,int len)
{
	int j;
	u8 any = 0;
	u8 positive = 'p';
	u8 negative = 'n';

	if(' ' == buf[0]){
		if(sts[0].val <= 0){
			sts[0].val = 1;
			digital_broadcast(ent,'a', stack,sp, &positive,1);
		}
		else{
			sts[0].val = -1;
			digital_broadcast(ent,'a', stack,sp, &negative,1);
		}
		return;
	}

	//step0: clear
	for(j=0;j<16;j++){
		if(0 == sts[j].cnt)break;
		sts[j].val = 0;
	}
	for(j=0;j<16;j++){
		if(0 == sts[j].cnt)break;
		digital_broadcast(ent,'a'+j, stack,sp, &any,0);
	}

	//step1: send
	switch(buf[0]){
	case '0':{
		sts[0].val = -1;
		digital_broadcast(ent,'a', stack,sp, &negative,0);
		sts[1].val = -1;
		digital_broadcast(ent,'b', stack,sp, &negative,0);
		break;
	}
	case '1':{
		sts[0].val = 1;
		digital_broadcast(ent,'a', stack,sp, &positive,0);
		sts[1].val = -1;
		digital_broadcast(ent,'b', stack,sp, &negative,0);
		break;
	}
	case '2':{
		sts[0].val = -1;
		digital_broadcast(ent,'a', stack,sp, &negative,0);
		sts[1].val = 1;
		digital_broadcast(ent,'b', stack,sp, &positive,0);
		break;
	}
	case '3':{
		sts[0].val = 1;
		digital_broadcast(ent,'a', stack,sp, &positive,0);
		sts[1].val = 1;
		digital_broadcast(ent,'b', stack,sp, &positive,0);
		break;
	}
	}

	//step2: chain write won't update all pin, so read 
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
			relation_readall(ent,'a'+j, stack,sp, 0,0, &val,1);
			if(0 == val)k += 1;
			else if('p' == val){
				sts[j].val = 1;
				digital_broadcast(ent,'a'+j, stack,sp, &positive,1);
			}
			else if('n' == val){
				sts[j].val =-1;
				digital_broadcast(ent,'a'+j, stack,sp, &negative,1);
			}
		}
		say("k=%d,err=%d\n", k,err);
		if(k == 0)break;	//all done
		if(k >= err)break;	//can not
		err = k;
	}
}




static u32 digital_color(int val)
{
	if(val < 0)return 0x0000ff;
	if(val > 0)return 0xff0000;
	return 0xffffff;
}
static void digital_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41solid_rect(wnd, 0x404040, vc, vr, vf);

	struct wireindex* sts = act->PERPIN;
	if(0 == sts)return;
	float* dat = act->VERTEX;
	if(0 == dat)return;

	int j,k;
	int off,cnt,rgb;
	vec3 ta,tb;
	for(k=0;k<0x10000/sizeof(struct wireindex);k++){
		cnt = sts[k].cnt;
		if(0 == cnt)break;

		off = sts[k].off;
		rgb = digital_color(sts[k].val);
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
void digital_read_board(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		digital_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}




int digital_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	digital_read_board(ent,foot, stack,sp, arg,key);
	return 0;
}
int digital_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	struct wireindex* sts = ent->PERPIN;
	if(0 == sts)return 0;

	switch(foot){
		case _evby_:
		{
			struct event* ev = (void*)buf;
			if(_char_ != ev->what)return 0;
			if(ev->why < '0')return 0;
			if(ev->why > '3')return 0;
			//not break
		}
		case _ioby_:
		{
			say("digital_event: %.4s=%x\n", &foot, buf[0]);
			ent->STAMP += 1;
			digital_complex(ent,sts, stack,sp, buf,len);
			break;
		}
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		{
			say("digital_write: %.4s=%x\n", &foot, buf[0]);
			int id = foot - 'a';
			if('p' == buf[0])sts[id].val = 1;
			if('n' == buf[0])sts[id].val =-1;
			digital_broadcast(ent,foot, stack,sp, buf,len);
			break;
		}
	}
	return 0;
}
int digital_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int digital_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@digital_linkup: %.4s\n", &self->flag);
	return 0;
}




int digital_search(struct entity* scene)
{
	return 0;
}
int digital_modify(struct entity* scene)
{
	return 0;
}
int digital_delete(struct entity* scene)
{
	return 0;
}
int digital_create(struct entity* scene, void* arg, int argc, u8** argv)
{
	int ret;
	say("@digital_create\n");
	if(0 == arg)return 0;

	//borrow this as filebuf
	scene->PERPIN = memorycreate(0x10000, 0);
	ret = openreadclose(arg, 0, scene->PERPIN, 0x10000);
	if(ret <= 0)return 0;

	//parse xyz
	scene->VERTEX = memorycreate(0x10000, 0);
	parsewiring(scene->PERPIN, scene->VERTEX);
	return 0;
}
