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
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);



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
static void piano_char(_obj* act, u8* ch)
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
		default: f = 0.00;break;
	}

	piano_gen(act->listptr.buf0, f);
	give_data_into_peer(act,_spk_, 0,0, 0,0, act->listptr.buf0,16384*2);
}
static void piano_event(_obj* act, struct event* ev)
{
	int j,k;
	float f;
	logtoall("@piano_event\n");

	if(_char_ == ev->what)piano_char(act, (void*)ev);
/*
	if(0x2d70 == ev->what){
		j = (ev->why)&0xffff;
		j = j*70/(win->whdf.width);
		logtoall("j=%d\n",j);

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
		logtoall("f=%f\n",f);

		piano_gen(act->buf, f);
		soundwrite(act, _spk_, 0, 0, act->buf, 16384*2);
	}//if(mouseup)
*/
}
static void piano_write_bywnd(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd;struct style* area;
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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
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




static void piano_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void piano_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void piano_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void piano_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("piano(%x,%x,%x)\n",win,act,sty);
}




static void piano_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	piano_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void piano_read_bywnd(_obj* ent,void* foot, _obj* wnd,void* area)
{
	struct fstyle fs;
	switch(wnd->vfmt){
	case _bgra8888_:
	case _rgba8888_:
		break;
	case _gl41list_:
		fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.5;
		fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
		fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
		fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-0.5;
		gl41data_before(wnd);
		piano_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
		gl41data_nocam(wnd);
		gl41data_after(wnd);
		break;
	}
}




static void piano_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		piano_read_bywnd(ent,foot, caller,area);
	default:
		piano_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void piano_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(_std_ == stack[sp-1].foottype){
		piano_char(ent, buf);
		return;
	}

	_obj* sup = stack[sp-2].pchip;
	switch(sup->type){
		case _gl41list_:piano_write_bywnd(ent,foot, stack,sp, arg,key, buf,len);break;
	}
}
static void piano_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void piano_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void piano_search(_obj* act)
{
}
static void piano_modify(_obj* act)
{
}
static void piano_delete(_obj* act)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memoryfree(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void piano_create(_obj* act)
{
	if(0 == act)return;

	act->listptr.buf0 = memoryalloc(0x100000, 0);
	if(0 == act->listptr.buf0)return;
}




void piano_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('p', 'i', 'a', 'n', 'o', 0, 0, 0);

	p->oncreate = (void*)piano_create;
	p->ondelete = (void*)piano_delete;
	p->onreader = (void*)piano_search;
	p->onwriter = (void*)piano_modify;

	p->onattach = (void*)piano_attach;
	p->ondetach = (void*)piano_detach;
	p->ontaking = (void*)piano_taking;
	p->ongiving = (void*)piano_giving;
}
