#include "libuser.h"
#define _corner_ hex64('c', 'o', 'r', 'n', 'e', 'r', 0, 0)
#define CLIENT whdf.iw0
#define SERVER whdf.iwn
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_whcam(_obj* wnd, struct style* area);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);




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
void drawterm(_obj* win, void* term, int x0, int y0, int x1, int y1);
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




void terminal_write_s(_obj* ent,struct style* slot, _syn* stack,int sp, u8* buf, int len)
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
		terminal_serverinput((void*)ent->priv_256b, buf, len);
	}
}
void terminal_write_c(_obj* ent,struct style* slot, _syn* stack,int sp, void* buf, int len)
{
	if(0 == ent->SERVER)input(buf, len);
	else give_data_into_peer(ent,'s', stack,sp, 0,0, buf,len);
}
static void terminal_write_bywnd(_obj* ent,struct style* slot, _syn* stack,int sp, struct event* ev,int len)
{
	u32 tmp;
	if(_char_ == ev->what){
		terminal_write_c(ent,slot, stack,sp, ev, 1);
		return;
	}
	if(_kbd_ == ev->what){
		switch(ev->why){
		case 0x48:
			if(ent->whdf.iy0 >= 10)ent->whdf.iy0 -= 10;
			return;
			//tmp = 0x415b1b;len = 3;
			//break;
		case 0x50:
			ent->whdf.iy0 += 10;
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
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty){
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}

	if(ent->orel0){
		drawopaque_rect(win, 0x111111, cx-ww, cy-hh, cx+ww, cy+hh);
		drawterm(win, (void*)ent->priv_256b, cx-ww, cy-hh, cx+ww, cy+hh);
	}
	else{
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
				if(k == ent->whdf.iy0){
					here = j+1;
					break;
				}
			}
		}


		//output bg color
		drawsolid_rect(win, 0x202020, cx-ww, cy-hh, cx+ww, cy+hh);

		//output text area
		int last = here + drawtext(win, 0xffffff, cx-ww, cy-hh, cx+ww, cy+hh, obuf+here, ocur-here);
	
		//output scroll bar
		int ytop = cy-hh + (2*hh*here)/ocur;
		int ybot = cy-hh + (2*hh*last)/ocur;
		drawopaque_rect(win, 0x80e0e0e0, cx+ww-16, ytop, cx+ww, ybot);


		//input bg color
		drawsolid_rect(win, 0x404040, cx, cy+hh-128, cx+ww-64, cy+hh);

		//input text area
		drawtext(win, 0xffffff, cx, cy+hh-128, cx+ww-64, cy+hh, (u8*)"~~~input line1~~~\n@@@input line2@@@\n", 0);

		//input scroll bar
	}
}
static void terminal_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void terminal_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void terminal_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x, y, w, h;
	u32* p;
	u32* q;
	u8* buf;
	struct uartterm* term = (void*)act->priv_256b;
	w = win->whdf.width;
	h = win->whdf.height;
	if(w > term->width)w = term->width;
	if(h > term->height)h = term->height;

	p = (void*)(win->tuitext.buf);
	q = (void*)(term->buf);
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			p[(win->whdf.width)*y + x] = q[(term->width)*(term->top+y) + x];
		}
	}
}
static void terminal_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u8* p;
	int enq, deq;
	//say("terminal(%x,%x,%x)\n",win,act,sty);
}




void gl41_vt100(_obj* wnd, struct uartterm* term, float* vc,float* vr,float* vf)
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
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
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
		gl41_vt100(wnd,(void*)act->priv_256b,vc,vr,vf);
	}
}




static void terminal_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* cam;struct style* camg;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	cam = stack[sp-4].pchip;camg = stack[sp-4].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	terminal_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void terminal_wrl_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* mgr;struct style* geom;
	_obj* wnd;struct style* area;
	mgr = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-4].pchip;area = stack[sp-4].pfoot;
	//say("mgr=%p,geom=%p,wnd=%p,area=%p\n", mgr, geom, wnd, area);

	//compute relative position from window
	int j;
	struct fstyle fs;
	if(area){
		for(j=0;j<3;j++)fs.vc[j] = fs.vr[j] = fs.vf[j] = fs.vt[j] = 0.0;
		fs.vr[0] = area->fs.vq[0] * wnd->whdf.fbwidth / 2.0;
		fs.vf[1] = area->fs.vq[1] * wnd->whdf.fbheight/ 2.0;
		fs.vt[2] = 1.0;
	}

	//draw it
	switch(wnd->hfmt){
	case _rgba_:
		terminal_draw_pixel(ent,slot, wnd,geom);
		break;
	default:
		terminal_draw_gl41(ent,slot, mgr,(void*)&fs, wnd,area);
	}
}
static void terminal_wnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	int j;
	struct fstyle fs;
	for(j=0;j<3;j++)fs.vc[j] = fs.vr[j] = fs.vf[j] = fs.vt[j] = 0.0;
	fs.vr[0] = area->fs.vq[0] * wnd->whdf.fbwidth / 2.0;
	fs.vf[1] = area->fs.vq[1] * wnd->whdf.fbheight/ 2.0;
	fs.vt[2] = 1.0;

	gl41data_before(wnd);
	terminal_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_whcam(wnd, area);
	gl41data_after(wnd);
}




static void terminal_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	case 'c':
		return;
	case 's':
		return;
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
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
static void terminal_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	switch(stack[sp-1].foottype){
	case 'c':terminal_write_c(ent,foot, stack,sp, buf,len);return;
	case 's':terminal_write_s(ent,foot, stack,sp, buf,len);return;
	case _evby_:terminal_write_bywnd(ent,foot, stack,sp, buf,len);return;
	}

	switch(wnd->hfmt){
	case _rgba_:
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
	case _wndmgr_:
		terminal_write_bywnd(ent,foot, stack,sp, buf,len);
		return;
	}
}
static void terminal_detach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	switch(self->foottype){
	case 'c':ent->CLIENT = 0;break;
	case 's':ent->SERVER = 0;break;
	}
}
static void terminal_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	switch(self->foottype){
	case 'c':ent->CLIENT = 1;break;
	case 's':ent->SERVER = 1;break;
	}
}




static void terminal_reader(_obj* ent,void* foot, p64 arg,int cmd, void* buf,int len)
{
}
static void terminal_writer(_obj* ent,void* foot, p64 arg,int cmd, void* buf,int len)
{
}
static void terminal_delete(_obj* act, void* arg, int argc, u8** argv)
{
	if(0 == act)return;
}
static void terminal_create(_obj* act, void* arg, int argc, u8** argv)
{
	if(0 == act)return;

	act->CLIENT = 0;
	act->SERVER = 0;

	act->whdf.ix0 = 0;
	act->whdf.iy0 = 100;

	//struct str* dat = act->RAWBUF = memorycreate(0x100000, 0);
	//dat->len = 0;

	struct uartterm* term = (void*)act->priv_256b;
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




void terminal_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('t', 'e', 'r', 'm');

	p->oncreate = (void*)terminal_create;
	p->ondelete = (void*)terminal_delete;
	p->onreader = (void*)terminal_reader;
	p->onwriter = (void*)terminal_writer;

	p->onattach = (void*)terminal_attach;
	p->ondetach = (void*)terminal_detach;
	p->ontaking = (void*)terminal_taking;
	p->ongiving = (void*)terminal_giving;
}
