#include "libuser.h"
#define _corner_ hex64('c', 'o', 'r', 'n', 'e', 'r', 0, 0)
#define CLIENT data0
#define SERVER data1
#define RAWBUF buf2
#define TTTBUF buf3
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_whcam(struct entity* wnd, struct style* area);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




struct uartterm
{
	u8* buf;
	u32 len;
	u32 fg;
	u32 bg;

	int width;
	int height;
	int vimw;
	int vimh;

	int left;
	int right;
	int top;
	int bottom;

	int curx;
	int cury;
};
//
void drawterm(struct entity* win, void* term, int x0, int y0, int x1, int y1);
void terminal_serverinput(struct uartterm* term, u8* buf, int len);
//
void* getstdin();
int getcurin();
void* getstdout();
int getcurout();
void input(u8* buf, int len);



static u32 colortable[16] = {
	0x101010, 0xe00000, 0x00e000, 0xe0e000, 0x0000e0, 0xe000e0, 0x00e0e0, 0xe0e0e0,
	0x000000, 0xff0000, 0x00ff00, 0xffff00, 0x0000ff, 0xff00ff, 0x00ffff, 0xffffff};




void terminal_write_s(_ent* ent,struct style* slot, _syn* stack,int sp, u8* buf, int len)
{
	//printmemory(buf, 8);

	if(ent->CLIENT){
		give_data_into_peer(ent,'c', stack,sp, 0,0, buf,len);
	}
	else{
/*		int j;
		struct str* dat = ent->RAWBUF;
		for(j=0;j<len;j++)dat->buf[dat->len + j] = buf[j];
		dat->len += j;*/
		terminal_serverinput(ent->TTTBUF, buf, len);
	}
}
void terminal_write_c(_ent* ent,struct style* slot, _syn* stack,int sp, void* buf, int len)
{
	if(0 == ent->SERVER)input(buf, len);
	else give_data_into_peer(ent,'s', stack,sp, 0,0, buf,len);
}
static void terminal_write_bywnd(_ent* ent,struct style* slot, _syn* stack,int sp, struct event* ev,int len)
{
	u32 tmp;
	if(_char_ == ev->what){
		terminal_write_c(ent,slot, stack,sp, ev, 1);
		return;
	}
	if(_kbd_ == ev->what){
		switch(ev->why){
		case 0x48:
			if(ent->iy0 >= 10)ent->iy0 -= 10;
			return;
			//tmp = 0x415b1b;len = 3;
			//break;
		case 0x50:
			ent->iy0 += 10;
			return;
			//tmp = 0x425b1b;len = 3;
			//break;
		case 0x4d:
			tmp = 0x435b1b;len = 3;
			break;
		case 0x4b:
			tmp = 0x445b1b;len = 3;
			break;
		case 0x1b:
			tmp = 0x1b;len = 1;
			break;
		default:
			return;
		}
		terminal_write_c(ent,slot, stack,sp, &tmp, len);
		return;
	}
}




static void terminal_draw_pixel(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty){
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	if(ent->orel0){
		drawopaque_rect(win, 0x111111, cx-ww, cy-hh, cx+ww, cy+hh);
		drawterm(win, ent->TTTBUF, cx-ww, cy-hh, cx+ww, cy+hh);
	}
	else{
		drawsolid_rect(win, 0x202020, cx-ww, cy-hh, cx+ww, cy+hh);

		u8* obuf = getstdout();
		if(0 == obuf)return;
		int ocur = getcurout();
		if(ocur <= 0)return;

		int j,k = 0;
		int here = 0;
		for(j=0;j<ocur;j++){
			if(obuf[j] < 0x8)break;
			if('\n' == obuf[j]){
				k++;
				if(k == ent->iy0){
					here = j+1;
					break;
				}
			}
		}

		int last = here + drawtext(win, 0xffffff, cx-ww, cy-hh, cx+ww, cy+hh, obuf+here, ocur-here);
	
		int ytop = cy-hh + (2*hh*here)/ocur;
		int ybot = cy-hh + (2*hh*last)/ocur;
		drawopaque_rect(win, 0x80e0e0e0, cx+ww-16, ytop, cx+ww, ybot);
	}
}
static void terminal_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terminal_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void terminal_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x, y, w, h;
	u32* p;
	u32* q;
	u8* buf;
	struct uartterm* term;

	term = act->TTTBUF;
	w = win->width;
	h = win->height;
	if(w > term->width)w = term->width;
	if(h > term->height)h = term->height;

	p = (void*)(win->textbuf);
	q = (void*)(term->buf);
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			p[(win->width)*y + x] = q[(term->width)*(term->top+y) + x];
		}
	}
}
static void terminal_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u8* p;
	int enq, deq;
	//say("terminal(%x,%x,%x)\n",win,act,sty);
}




void gl41_vt100(struct entity* wnd, struct uartterm* term, float* vc,float* vr,float* vf)
{
	int x,y,j,rgb;
	vec3 tr,tf;
	vec3 tc,qc;
	for(j=0;j<3;j++){tr[j]=vr[j];tf[j]=vf[j];}
	vec3_setlen(tr, 32);
	vec3_setlen(tf, 32);

	//printmemory(term->buf, term->width);
	u8* buf;
	for(y=0;y<32;y++){
		buf = &term->buf[(term->top+y)*term->width*4];
		for(x=0;x<80;x++){
			for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]+tr[j]*x/2 +vf[j]-tf[j]*y;
			if(buf[x*4]<0x80){
				rgb = colortable[buf[x*4+2]&0xf];
				gl41ascii(wnd, rgb, tc,tr,tf, buf[x*4]);
			}
			else{
				rgb = colortable[buf[x*4+6]&0xf];
				gl41utf8(wnd, rgb, tc,tr,tf, &buf[x*4], 0);
				x++;
			}
		}
	}
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]+tr[j]*(term->curx)/2 +vf[j]-tf[j]*(term->cury - term->top);
		qc[j] = tc[j] +tf[j];
	}
	gl41line(wnd, 0xffffff, tc,qc);
}
static void terminal_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tc;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	for(j=0;j<3;j++)tc[j] = vc[j] - vt[j]/1000;
	gl41opaque_rect(wnd, 0x80101010, tc, vr, vf);

	if(0 == act->SERVER){
		void* obuf = getstdout();
		int ocur = getcurout();
		gl41text_reverse(wnd, 0xffffff, vc, vr, vf, obuf, ocur);
	}
	else{
/*		struct str* dat = act->RAWBUF;
		gl41text(wnd, 0xffffff, vc,vr,vf, dat->buf, dat->len);*/

		struct uartterm* term = act->TTTBUF;
		if(0 == term)return;
		gl41_vt100(wnd,term,vc,vr,vf);
	}
}




static void terminal_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* cam;struct style* camg;
	struct entity* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	cam = stack[sp-4].pchip;camg = stack[sp-4].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	terminal_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void terminal_wrl_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* mgr;struct style* geom;
	struct entity* wnd;struct style* area;
	mgr = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-4].pchip;area = stack[sp-4].pfoot;

	int j;
	struct fstyle fs;
	for(j=0;j<3;j++)fs.vc[j] = fs.vr[j] = fs.vf[j] = fs.vt[j] = 0.0;
	fs.vr[0] = area->fs.vq[0] * wnd->fbwidth / 2.0;
	fs.vf[1] = area->fs.vq[1] * wnd->fbheight/ 2.0;
	fs.vt[2] = 1.0;
	terminal_draw_gl41(ent,slot, mgr,(void*)&fs, wnd,area);
}
static void terminal_wnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area)
{
	int j;
	struct fstyle fs;
	for(j=0;j<3;j++)fs.vc[j] = fs.vr[j] = fs.vf[j] = fs.vt[j] = 0.0;
	fs.vr[0] = area->fs.vq[0] * wnd->fbwidth / 2.0;
	fs.vf[1] = area->fs.vq[1] * wnd->fbheight/ 2.0;
	fs.vt[2] = 1.0;

	gl41data_before(wnd);
	terminal_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_whcam(wnd, area);
	gl41data_after(wnd);
}




static void terminal_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	case 'c':
		return;
	case 's':
		return;
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		terminal_draw_pixel(ent,slot, caller,area);
		break;
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		terminal_wnd(ent,slot, caller,area);
		break;
	case _corner_:
	case _wndmgr_:
		terminal_wrl_wnd(ent,slot, stack,sp);
		break;
	default:
		terminal_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void terminal_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	switch(stack[sp-1].flag){
	case 'c':terminal_write_c(ent,foot, stack,sp, buf,len);return;
	case 's':terminal_write_s(ent,foot, stack,sp, buf,len);return;
	case _evby_:terminal_write_bywnd(ent,foot, stack,sp, buf,len);return;
	}

	switch(wnd->fmt){
	case _rgba_:
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		terminal_write_bywnd(ent,foot, stack,sp, buf,len);
		return;
	}
}
static void terminal_discon(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
	case 'c':ent->CLIENT = 0;break;
	case 's':ent->SERVER = 0;break;
	}
}
static void terminal_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
	case 'c':ent->CLIENT = 1;break;
	case 's':ent->SERVER = 1;break;
	}
}




static void terminal_search(struct entity* act)
{
}
static void terminal_modify(struct entity* act)
{
}
static void terminal_delete(struct entity* act)
{
	if(0 == act)return;
}
static void terminal_create(struct entity* act, void* arg, int argc, u8** argv)
{
	if(0 == act)return;

	act->CLIENT = 0;
	act->SERVER = 0;

	act->ix0 = 0;
	act->iy0 = 840;

	//struct str* dat = act->RAWBUF = memorycreate(0x100000, 0);
	//dat->len = 0;

	struct uartterm* term = act->TTTBUF = memorycreate(sizeof(struct uartterm), 0);
	term->curx = 0;
	term->cury = 0;
	term->left = 0;
	term->right = 0;
	term->top = 0;
	term->bottom = 0;
	term->vimw = 128;
	term->vimh = 24;
	term->width = 128;
	term->height = 24;
	term->bg = 0;
	term->fg = 7;
	term->len = 0x100000;
	term->buf = memorycreate(0x100000, 0);
}




void terminal_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('t', 'e', 'r', 'm');

	p->oncreate = (void*)terminal_create;
	p->ondelete = (void*)terminal_delete;
	p->onsearch = (void*)terminal_search;
	p->onmodify = (void*)terminal_modify;

	p->onlinkup = (void*)terminal_linkup;
	p->ondiscon = (void*)terminal_discon;
	p->ontaking = (void*)terminal_taking;
	p->ongiving = (void*)terminal_giving;
}
