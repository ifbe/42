#include "libuser.h"




void vkbd_draw_pixel(struct arena* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,x0,y0,x1,y1;
	int w = win->width;
	int h = win->height;
	//if(win->vkbdw < 0)return;

    //drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);

    for(y=0;y<8;y++)
    {
        for(x=0;x<16;x++)
        {
            l = 2;
            c = x+(y<<4);
            //if(c == (win->vkbdz))rgb = 0xffff00ff;
            //else rgb = 0x20808080;

            //joystick area
            if((y>8)&&(y<15))
            {
                if((x>0)&&(x<7))continue;
                if((x>8)&&(x<15))continue;
            }

            if(0x0 == c)c = hex32('\\','0',0,0);
            else if(0x7 == c)c = hex32('\\','a',0,0);
            else if(0x8 == c)c = hex32('\\','b',0,0);
            else if(0x9 == c)c = hex32('\\','t',0,0);
            else if(0xa == c)c = hex32('\\','n',0,0);
            else if(0xd == c)c = hex32('\\','r',0,0);
            else if(0xf0 <= c)
            {
                if(0xfa <= c)
                {
                    l = 3;
                    c = ((c-0xfa)<<16) + hex32('f','1','0',0);
                }
                else
                {
                    l = 2;
                    c = ((c-0xf0)<<8) + hex32('f','0',0,0);
                }
            }
            else if(0x80 <= c)
            {
                l = 1;
                c = ' ';
            }
            else l = 1;

            x0 = (x*w/16)+1;
            y0 = h+1-((y+1)*h/32)+1;
            x1 = ((x+1)*w/16)-1;
            y1 = h-1-(y*h/32);
            drawhyaline_rect(win, 0x7fffffff, x0, y0, x1, y1);
	        drawstring_fit(win, rgb, x0, y0, x1, y1, (u8*)&c, l);
        }
    }
}
void vkbd_draw_vbo(struct arena* win, struct style* sty)
{
	u8 ch[8];
	float j,k;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	int x,y,c,rgb;
	int w = win->width;
	int h = win->height;
/*
	if(win->vkbdw < 0)return;
	c = win->vkbdw;
	if(('j' == c)|('k' == c))
	{
		vc[0] = 0.0;
		vc[1] = -0.75;
		vc[2] = -0.5;
		vr[0] = 1.0;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = 0.25;
		vf[2] = 0.0;
		carvesolid2d_rect(win, 0x202020, vc, vr, vf);
	}
*/
    if(w<h)x = w/17;
    else x = h/17;
    j = (float)x / (float)w;
    k = (float)x / (float)h;

    for(y=0;y<8;y++)
    {
        for(x=0;x<16;x++)
        {
            c = x+(y<<4);
            //if(c == (win->vkbdz))rgb = 0xff00ff;
            //else rgb = 0x808080;
			rgb = 0x80808080;

            vc[0] = (x-7.5)/8.0;
            vc[1] = (y-15.5)/16.0;
            vc[2] = -0.9;
            vr[0] = 1.0/17;
            vr[1] = 0.0;
            vr[2] = 0.0;
            vf[0] = 0.0;
            vf[1] = 0.5/17;
            vf[2] = 0.0;
            carveopaque2d_rect(win, rgb, vc, vr, vf);

            vc[2] = -0.91;
            vr[0] = j;
            vf[1] = k/2;
            if((0==c)|(7==c)|(8==c)|(9==c)|(0xa==c)|(0xd==c))
            {
                if(0x0 == c)c = '0';
                else if(0x7 == c)c = 'a';
                else if(0x8 == c)c = 'b';
                else if(0x9 == c)c = 't';
                else if(0xa == c)c = 'n';
                else if(0xd == c)c = 'r';
                vc[0] = (x-7.5)/8.0+j;
                vc[1] = (y-15.5)/16.0;
                carve2d_ascii(win, 0xffffff, vc, vr, vf, c);
                c = '\\';
            }

            vc[0] = (x-7.5)/8.0;
            vc[1] = (y-15.5)/16.0;
            carve2d_ascii(win, 0xffffff, vc, vr, vf, c);
        }
    }
}
void vkbd_draw_html(struct arena* win, struct style* sty)
{
}
void vkbd_draw_tui(struct arena* win, struct style* sty)
{
}
void vkbd_draw_cli(struct arena* win, struct style* sty)
{
}
static void vkbd_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)vkbd_draw_cli(win, sty);
	else if(fmt == _tui_)vkbd_draw_tui(win, sty);
	else if(fmt == _html_)vkbd_draw_html(win, sty);
	else if(fmt == _vbo_)vkbd_draw_vbo(win, sty);
	else vkbd_draw_pixel(win, sty);
}
static int vkbd_event(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	short tmp[4];
	int x,y,w,h,ret;
    //say("vkbd_keyboard_write\n");
	//if(win->vkbdw <= 0)return 0;

	w = win->width;
	h = win->height;
	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	if(y < h*3/4)return 0;

	if(hex32('p','-',0,0) == ev->what)
	{
		x = 16*x/w;
		y = 31 - 32*y/h;
		//say("x=%d,y=%d\n",x,y);
		eventwrite(x+(y*16), _char_, (u64)win, 0);
	}
	return 1;
}




static void vkbd_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	vkbd_draw(act, pin, win, sty);
}
static int vkbd_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return vkbd_event(act, pin, win, sty, ev, 0);
}
static int vkbd_cread(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	return 0;
}
static int vkbd_cwrite(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	return 0;
}
static int vkbd_stop(
	struct actor* leaf, struct style* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	return 0;
}
static int vkbd_start(
	struct actor* leaf, struct style* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	return 0;
}
static int vkbd_delete(struct arena* win)
{
	return 0;
}
static int vkbd_create(struct arena* win, u8* str)
{
	return 0;
}




void vkbd_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('v', 'k', 'b', 'd');

	p->oncreate = (void*)vkbd_create;
	p->ondelete = (void*)vkbd_delete;
	p->onstart  = (void*)vkbd_start;
	p->onstop   = (void*)vkbd_stop;
	p->oncread  = (void*)vkbd_cread;
	p->oncwrite = (void*)vkbd_cwrite;
	p->onsread  = (void*)vkbd_sread;
	p->onswrite = (void*)vkbd_swrite;
}