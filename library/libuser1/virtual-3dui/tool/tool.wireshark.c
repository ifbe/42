#include "libuser.h"
#define _UDP_ hex32('U','D','P',0)




void queuepacket(u8* dst, int* idx, u8* buf, int len)
{
	int j;
	for(j=0;j<len;j++)dst[j] = buf[j];
}




static void rawdump_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int j;
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
	drawline_rect(win, 0xffffff, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
}
static void rawdump_draw_gl41(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rawdump_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rawdump_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rawdump_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void rawdump_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void rawdump_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void rawdump_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void rawdump_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void rawdump_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void rawdump_search(_obj* act, u8* buf)
{
}
static void rawdump_modify(_obj* act, u8* buf)
{
}
static void rawdump_delete(_obj* act, u8* buf)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memoryfree(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
	if(act->listptr.buf1){
		memoryfree(act->listptr.buf1);
		act->listptr.buf1 = 0;
	}
}
static void rawdump_create(_obj* act, u8* buf)
{
	if(0 == act)return;
	act->listptr.buf1 = memoryalloc(0x10000, 0);
	act->listptr.buf0 = memoryalloc(0x100000, 0);
}




void rawdump_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('r','a','w','d','u','m','p',0);

	p->oncreate = (void*)rawdump_create;
	p->ondelete = (void*)rawdump_delete;
	p->onreader = (void*)rawdump_search;
	p->onwriter = (void*)rawdump_modify;

	p->onattach = (void*)rawdump_attach;
	p->ondetach = (void*)rawdump_detach;
	p->ontaking = (void*)rawdump_taking;
	p->ongiving = (void*)rawdump_giving;
}
