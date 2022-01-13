#include "libuser.h"
int parsefv(float* vec, int flen, u8* str, int slen);




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
static void analog_emulate(_obj* ent, struct wireindex* sts, u8* buf, int len)
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
	give_data_into_them(ent,'a', 0,0, 0,0, &sts[0],0);
}
static void analog_decent_V(_obj* ent, struct wireindex* sts)
{
	int j;
	for(j=1;j<16;j++){
		if(0 == sts[j].cnt)break;
		if(0 != sts[j].sure)continue;

		sts[j].grad = 0.0;
		take_data_from_them(ent,'a'+j, 0,0, 0,'V', sts,j);
	}
	for(j=1;j<16;j++){
		if(0 == sts[j].cnt)break;
		if(0 != sts[j].sure)continue;

		sts[j].volt -= sts[j].grad;
		say("%f\n",sts[j].volt);
	}
}
static void analog_decent_R(_obj* ent, struct wireindex* sts)
{
	int j;
	for(j=1;j<16;j++){
		if(0 == sts[j].cnt)break;
		if(0 != sts[j].sure)continue;

		sts[j].grad = 0.0;
		take_data_from_them(ent,'a'+j, 0,0, 0,'R', sts,j);
	}
	for(j=1;j<16;j++){
		if(0 == sts[j].cnt)break;
		if(0 != sts[j].sure)continue;

		sts[j].volt -= sts[j].grad;
		say("%f\n",sts[j].volt);
	}
}




static void analog_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41solid_rect(wnd, 0x404040, vc, vr, vf);

	struct wireindex* sts = act->listptr.buf0;
	if(0 == sts)return;
	float* dat = act->listptr.buf1;
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
		gl41float(wnd, rgb, tc,tr,tf, sts[k].volt);
	}
}
void analog_read_board(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	analog_draw_gl41(ent,slot, wor,geom, wnd,area);
}
int analog_read_child(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(rel->srcfoot){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].foottype = rel->srcfoottype;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].foottype = rel->dstfoottype;
			entity_take(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
		}
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int analog_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	}

	analog_read_child(ent,foot, stack,sp, arg,key, buf,len);
	analog_read_board(ent,foot, stack,sp, arg,key, buf,len);
	return 0;
}
int analog_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct wireindex* sts = ent->listptr.buf0;
	if(0 == sts)return 0;

	struct event* ev;
	int id;
say("analog_write: %.4s\n", &foot);
	switch(stack[sp-1].foottype){
	case _evby_:
		ev = buf;
		if(_char_ != ev->what)return 0;
		if(ev->why < '0')return 0;
		if(ev->why > '9')return 0;
		//not break
	case _ioby_:
		analog_emulate(ent,sts, buf,len);
		break;
	case 'a':
	case 'b':
	case 'c':
	case 'd':
		id = stack[sp-1].foottype - 'a';
		analog_voltage(&sts[id], buf);
		break;
	}
	return 0;
}
int analog_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int analog_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@analog_attach: %.4s\n", &self->foottype);
	return 0;
}




int analog_search(_obj* scene)
{
	return 0;
}
int analog_modify(_obj* scene)
{
	return 0;
}
int analog_delete(_obj* scene)
{
	return 0;
}
int analog_create(_obj* scene, void* arg, int argc, u8** argv)
{
	int ret;
	say("@analog_create\n");
	if(0 == arg)return 0;

	scene->listptr.buf0 = memorycreate(0x10000, 0);
	ret = openreadclose(arg, 0, scene->listptr.buf0, 0x10000);
	if(ret <= 0)return 0;

	scene->listptr.buf1 = memorycreate(0x10000, 0);
	parsewiring(scene->listptr.buf0, scene->listptr.buf1);
	return 0;
}
