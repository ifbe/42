#include "libuser.h"
#define cccc 440.000
#define c__d 466.164
#define dddd 493.883
#define d__e 523.251
#define eeee 554.365
#define ffff 587.330
#define f__g 622.254
#define gggg 659.255
#define g__a 698.456
#define aaaa 739.989
#define a__b 783.991
#define bbbb 830.609
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);



static void piano_gen(short* pcm, float f)
{
	int j;
	for(j=0;j<16384;j++)
	{
		pcm[j]  =16384 * getsin(j*2*PI*f*1/44100)
			+ 8192 * getsin(j*2*PI*f*2/44100)
			+ 1024 * getsin(j*2*PI*f*3/44100)
			+ 4096 * getsin(j*2*PI*f*4/44100)
			+ 1024 * getsin(j*2*PI*f*5/44100);

		pcm[j] *= 1.0 - j/16384.0;
	}
}
static void piano_char(struct entity* act, u8* ch)
{
	float f;
	switch(ch[0]){
		case 'c':f = cccc;break;
		case 'd':f = dddd;break;
		case 'e':f = eeee;break;
		case 'f':f = ffff;break;
		case 'g':f = gggg;break;
		case 'a':f = aaaa;break;
		case 'b':f = bbbb;break;
		case '1':f = cccc;break;
		case '2':f = c__d;break;
		case '3':f = dddd;break;
		case '4':f = d__e;break;
		case '5':f = eeee;break;
		case '6':f = ffff;break;
		case '7':f = f__g;break;
		case '8':f = gggg;break;
		case '9':f = g__a;break;
		case '0':f = aaaa;break;
		case '-':f = a__b;break;
		case '=':f = bbbb;break;
		default:break;
	}

	piano_gen(act->buf0, f);
	give_data_into_peer(act,_spk_, 0,0, 0,0, act->buf0,16384*2);
}
static void piano_event(struct entity* act, struct event* ev)
{
	int j,k;
	float f;
	say("@piano_event\n");

	if(_char_ == ev->what)piano_char(act, (void*)ev);
/*
	if(0x2d70 == ev->what){
		j = (ev->why)&0xffff;
		j = j*70/(win->width);
		say("j=%d\n",j);

		k = j%7;
		j = j/7;
		switch(k)
		{
			case 0:f = cccc;break;
			case 1:f = dddd;break;
			case 2:f = eeee;break;
			case 3:f = ffff;break;
			case 4:f = gggg;break;
			case 5:f = aaaa;break;
			case 6:f = bbbb;break;
		}
		switch(j)
		{
			case 0:f /= 32;break;
			case 1:f /= 16;break;
			case 2:f /= 8;break;
			case 3:f /= 4;break;
			case 4:f /= 2;break;
			case 5:break;
			case 6:f *= 2;break;
			case 7:f *= 4;break;
			case 8:f *= 8;break;
			case 9:f *= 16;break;
		}
		say("f=%f\n",f);

		piano_gen(act->buf, f);
		soundwrite(act, _spk_, 0, 0, act->buf, 16384*2);
	}//if(mouseup)
*/
}
static void piano_write_bywnd(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct event* ev = buf;
	if(_char_ == ev->what){
		piano_event(ent, ev);
	}
	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
	}
}



/*
static void piano_draw_vbo2d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x;
	vec3 tc,tr,tf,tu;
	if(0 == sty)sty = defaultstyle_vbo2d();
	float* vc = sty->fs.vc;
	float* vr = sty->fs.vr;
	float* vf = sty->fs.vf;
	float* vu = sty->fs.vt;

	tr[0] = vr[0] / 7 / 11;
	tr[1] = vr[1] / 7 / 11;
	tr[2] = vr[2] / 7 / 11;
	tf[0] = vf[0]/4;
	tf[1] = vf[1]/4;
	tf[2] = vf[2]/4;
	for(x=0;x<7*10;x++)
	{
		tc[0] = vc[0] + vr[0]*(x-34.5)/35 - vf[0]/4;
		tc[1] = vc[1] + vr[1]*(x-34.5)/35 - vf[1]/4;
		tc[2] = vc[2] + vr[2]*(x-34.5)/35 - vf[2]/4;
		gl41solid2d_rect(win, 0xfffffff, tc, tr, tf);
	}

	tr[0] = vr[0] / 7 / 20;
	tr[1] = vr[1] / 7 / 20;
	tr[2] = vr[2] / 7 / 20;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	for(x=0;x<7*10;x++)
	{
		if((x%7) == 0)continue;
		if((x%7) == 3)continue;
		tc[0] = vc[0] + vr[0]*(x-35)/35 - vf[0]/8;
		tc[1] = vc[1] + vr[1]*(x-35)/35 - vf[1]/8;
		tc[2] = vc[2] + vr[2]*(x-35)/35 - vf[2]/8 - 0.1;
		gl41solid2d_rect(win, 0x202020, tc, tr, tf);
	}

	tc[0] = vc[0] + vr[0]*3/35 + vf[0]/4;
	tc[1] = vc[1] + vr[1]*3/35 + vf[1]/4;
	tc[2] = vc[2] + vr[2]*3/35 + vf[2]/4;
	tr[0] = vr[0] * 2*52 / 7 / 20;
	tr[1] = vr[1] * 2*52 / 7 / 20;
	tr[2] = vr[2] * 2*52 / 7 / 20;
	tf[0] = vf[0] / 4;
	tf[1] = vf[1] / 4;
	tf[2] = vf[2] / 4;
	gl41solid2d_rect(win, 0x404040, tc, tr, tf);

	tr[0] = vr[0] / 64;
	tr[1] = vr[1] / 64;
	tr[2] = vr[2] / 64;
	tf[0] = vf[0] / 64;
	tf[1] = vf[1] / 64;
	tf[2] = vf[2] / 64;
	tc[0] = vc[0] - vr[0]*16/20 + vf[0]/64;
	tc[1] = vc[1] - vr[1]*16/20 + vf[1]/64;
	tc[2] = vc[2] - vr[2]*16/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"27.5", 4);
	tc[0] = vc[0] - vr[0]*12/20 + vf[0]/64;
	tc[1] = vc[1] - vr[1]*12/20 + vf[1]/64;
	tc[2] = vc[2] - vr[2]*12/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"55", 2);
	tc[0] = vc[0] - vr[0]*8/20 + vf[0]/64;
	tc[1] = vc[1] - vr[1]*8/20 + vf[1]/64;
	tc[2] = vc[2] - vr[2]*8/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"110", 3);
	tc[0] = vc[0] - vr[0]*4/20 + vf[0]/64;
	tc[1] = vc[1] - vr[1]*4/20 + vf[1]/64;
	tc[2] = vc[2] - vr[2]*4/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"220", 3);
	tc[0] = vc[0] + vf[0]/64;
	tc[1] = vc[1] + vf[1]/64;
	tc[2] = vc[2] + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"440", 3);
	tc[0] = vc[0] + vr[0]*4/20 + vf[0]/64;
	tc[1] = vc[1] + vr[1]*4/20 + vf[1]/64;
	tc[2] = vc[2] + vr[2]*4/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"880", 3);
	tc[0] = vc[0] + vr[0]*8/20 + vf[0]/64;
	tc[1] = vc[1] + vr[1]*8/20 + vf[1]/64;
	tc[2] = vc[2] + vr[2]*8/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"1760", 4);
	tc[0] = vc[0] + vr[0]*12/20 + vf[0]/64;
	tc[1] = vc[1] + vr[1]*12/20 + vf[1]/64;
	tc[2] = vc[2] + vr[2]*12/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"3520", 4);
	tc[0] = vc[0] + vr[0]*16/20 + vf[0]/64;
	tc[1] = vc[1] + vr[1]*16/20 + vf[1]/64;
	tc[2] = vc[2] + vr[2]*16/20 + vf[2]/64 - 0.01;
	carve2d_string(win, 0xffffff, tc, tr, tf, (u8*)"7040", 4);
}*/
static void piano_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,j;
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
/*	tc[0] = vc[0] + vf[0]/2 + vu[0]/2;
	tc[1] = vc[1] + vf[1]/2 + vu[1]/2;
	tc[2] = vc[2] + vf[2]/2 + vu[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tu[0] = vu[0]/2;
	tu[1] = vu[1]/2;
	tu[2] = vu[2]/2;
	gl41solid_prism4(win, 0x202020, tc, vr, tf, tu);*/

	for(j=0;j<3;j++){
		tr[j] = vr[j] / 7 / 11;
		tf[j] = vf[j]/4;
	}
	for(x=0;x<7*10;x++)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] + vr[j]*(x-34.5)/35 - vf[j]/4 + vu[j]/2;
		gl41solid_rect(wnd, 0xfffffff, tc, tr, tf);
	}

	tr[0] = vr[0] / 7 / 20;
	tr[1] = vr[1] / 7 / 20;
	tr[2] = vr[2] / 7 / 20;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	for(x=0;x<7*10;x++)
	{
		if((x%7) == 0)continue;
		if((x%7) == 3)continue;
		for(j=0;j<3;j++)tc[j] = vc[j] + vr[j]*(x-35)/35 - vf[j]/8 + vu[j]*0.51;
		gl41solid_rect(wnd, 0x202020, tc, tr, tf);
	}
}
static void piano_read_bywnd(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	struct entity* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	gl41data_before(wnd);
	piano_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}




static void piano_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	int j;
	for(j=0;j<7;j++)
	{
		drawsolid_rect(
			win, 0xffffff,
			(cx-ww+4)+(2*j+0)*ww/7, (cy+4-hh),
			(cx-ww-4)+(2*j+2)*ww/7, (cy-4+hh)
		);
	}
	for(j=0;j<6;j++)
	{
		if(2 == j)continue;
		drawsolid_rect(
			win, 0x111111,
			(cx-ww)+(4*j+3)*ww/14, (cy+4-hh),
			(cx-ww)+(4*j+5)*ww/14, (cy-4)
		);
	}
}
static void piano_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void piano_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void piano_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void piano_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("piano(%x,%x,%x)\n",win,act,sty);
}




static void piano_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct supply* sup = stack[sp-2].pchip;
	switch(sup->fmt){
	case _gl41full_:{
		if('v' != key)break;
		piano_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	}
}
static void piano_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_std_ == stack[sp-1].flag){
		piano_char(ent, buf);
		return;
	}

	struct supply* sup = stack[sp-2].pchip;
	switch(sup->fmt){
		case _gl41full_:piano_write_bywnd(ent,foot, stack,sp, arg,key, buf,len);break;
	}
}
static void piano_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void piano_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void piano_search(struct entity* act)
{
}
static void piano_modify(struct entity* act)
{
}
static void piano_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void piano_create(struct entity* act)
{
	if(0 == act)return;

	act->buf0 = memorycreate(0x100000, 0);
	if(0 == act->buf0)return;
}




void piano_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'i', 'a', 'n', 'o', 0, 0, 0);

	p->oncreate = (void*)piano_create;
	p->ondelete = (void*)piano_delete;
	p->onsearch = (void*)piano_search;
	p->onmodify = (void*)piano_modify;

	p->onlinkup = (void*)piano_linkup;
	p->ondiscon = (void*)piano_discon;
	p->ontaking = (void*)piano_taking;
	p->ongiving = (void*)piano_giving;
}
