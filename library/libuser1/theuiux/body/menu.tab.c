#include "libuser.h"




void tabbar_vbo_listtwig(struct arena* win, struct style* sty, struct arena* tmp, int t)
{
    int x,y,j,rgb;
    vec3 rr;
    vec3 tc, tr, tf;
    struct relation* rel;
    struct actor* ac;
    struct style* st;

    tc[0] = (2*t-7)/16.0;
    tc[1] = -0.75;
    tc[2] = -0.8;
    tr[0] = 1.0 / 16;
    tr[1] = 0.0;
    tr[2] = 0.0;
    tf[0] = 0.0;
    tf[1] = 0.25;
    tf[2] = 0.0;
    carvesolid2d_rect(win, 0x808080, tc, tr, tf);

    tc[0] = 0.0;
    tc[1] = 0.0;
    tc[2] = -0.8;
    tr[0] = 0.5;
    tr[1] = 0.0;
    tr[2] = 0.0;
    tf[0] = 0.0;
    tf[1] = 0.5;
    tf[2] = 0.0;
    carvesolid2d_rect(win, 0x808080, tc, tr, tf);

    tr[0] = 1.0/18;
    tr[1] = 0.0;
    tr[2] = 0.0;
    tf[0] = 0.0;
    tf[1] = 1.0/18;
    tf[2] = 0.0;
    tc[2] = -0.9;
    rr[0] = 1.0/32;
    rr[1] = 0.0;
    rr[2] = 0.0;

    j = 0;
    rel = tmp->orel0;
    while(1){
        if(0 == rel)break;
        if(_act_ == rel->dsttype){
            x = j%8;
            y = j/8;
            tc[0] = (2*x-7)/16.0;
            tc[1] = (7-2*y)/16.0;
            carveopaque2d_rect(win, 0xffffff, tc, tr, tf);

            ac = (void*)(rel->dstchip);
            st = (void*)(rel->srcfoot);
            if('#' == st->uc[3])rgb = 0x404040;
            else rgb = 0xff00ff;
            carvestring2d_center(win, rgb, tc, rr, tf, (void*)&ac->fmt, 8);

            j++;
            if(j >= 64)break;
        }
        rel = samesrcnextdst(rel);
    }
}
void tabbar_vbo_listroot(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
    int j,rgb;
    vec3 pc,pf;
    vec3 tc,tr,tf;
    struct relation* rel;
    struct style* st;
    struct arena* aa;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

    tc[0] = vc[0] - vf[0]*31/32;
    tc[1] = vc[1] - vf[1]*31/32;
    tc[2] = vc[2] - vf[2]*31/32 - 0.5;
    tr[0] = vr[0] / 2;
    tr[1] = vr[1] / 2;
    tr[2] = vr[2] / 2;
    tf[0] = vf[0] / 32;
    tf[1] = vf[1] / 32;
    tf[2] = vf[2] / 32;
	carveline2d_rect(win, 0xff0000, tc, tr, tf);

    j = 0;
    rel = win->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_win_ == rel->dsttype){
            tc[0] = vc[0] + vr[0]*(2*j-7)/16 - vf[0]*31/32;
            tc[1] = vc[1] + vr[1]*(2*j-7)/16 - vf[1]*31/32;
            tc[2] = vc[2] + vr[2]*(2*j-7)/16 - vf[2]*31/32 - 0.8;
            tr[0] = vr[0] / 2 / 8.1;
            tr[1] = vr[1] / 2 / 8.1;
            tr[2] = vr[2] / 2 / 8.1;
            carvesolid2d_rect(win, 0x000080, tc, tr, tf);

            tc[2] -= 0.1;
            tr[0] = vr[0] / 2 / 16;
            tr[1] = vr[1] / 2 / 16;
            tr[2] = vr[2] / 2 / 16;

            aa = (void*)(rel->dstchip);
            carvestring2d_center(win, 0xffffff, tc, tr, tf, (void*)(&aa->fmt), 8);

            if(j == act->x0)tabbar_vbo_listtwig(win, sty, aa, j);

            j++;
            if(j == 8)break;
        }

        rel = samesrcnextdst(rel);
    }
}
void tabbar_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	if(0 == sty)sty = defaultstyle_vbo2d();

    //root
    tabbar_vbo_listroot(act, pin, win, sty);
}




void tabbar_pixel_listtwig(struct arena* win, struct style* sty, struct arena* tmp, int t)
{
    int x,y,j,rgb;
    struct relation* rel;
    struct actor* ac;
    struct style* st;
    int w = win->width;
    int h = win->height;

    drawsolid_rect(win, 0x808080, (t+4)*w/16, h*3/4, (t+5)*w/16, h*31/32);
    drawsolid_rect(win, 0x808080, w/4, h/4, w*3/4, h*3/4);

    j = 0;
    rel = tmp->orel0;
    while(1){
        if(0 == rel)break;
        if(_act_ == rel->dsttype){
            x = j%8;
            y = j/8;
            drawsolid_rect(
                win, 0xffffff,
                (x+4)*w/16, (y+4)*h/16,
                (x+5)*w/16, (y+5)*h/16
            );

            ac = (void*)(rel->dstchip);
            st = (void*)(rel->srcfoot);
            if('#' == st->uc[3])rgb = 0x404040;
            else rgb = 0xff00ff;
            drawstring_fit(
                win, rgb,
                (x+4)*w/16+2, (y+4)*h/16+2,
                (x+5)*w/16-2, (y+5)*h/16-2,
                (void*)&ac->fmt, 8
            );

            j++;
            if(j >= 64)break;
        }
        rel = samesrcnextdst(rel);
    }
}
void tabbar_pixel_listroot(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
    int j,rgb;
    struct relation* rel;
    struct style* st;
    struct arena* aa;
    int w = win->width;
    int h = win->height;

    j = 0;
    rel = win->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_win_ == rel->dsttype){
            drawsolid_rect(
                win, 0x000080,
                (j+4)*w/16, h*31/32,
                (j+5)*w/16, h-1
            );

            aa = (void*)(rel->dstchip);
            drawstring_fit(
                win, 0xffffff,
                (j+4)*w/16, h*31/32,
                (j+5)*w/16, h-1,
                (void*)(&aa->fmt), 8);

            if(j == act->x0)tabbar_pixel_listtwig(win, sty, aa, j);

            j++;
            if(j == 8)break;
        }

        rel = samesrcnextdst(rel);
    }
}
void tabbar_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
    tabbar_pixel_listroot(act, pin, win, sty);
}




void tabbar_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void tabbar_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void tabbar_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void tabbar_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tabbar_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(_cli_ == fmt)tabbar_read_cli(win, sty, act, pin);
	else if(_tui_ == fmt)tabbar_read_tui(win, sty, act, pin);
	else if(_html_ == fmt)tabbar_read_html(win, sty, act, pin);
	else if(_json_ == fmt)tabbar_read_json(win, sty, act, pin);
	else if(_vbo_ == fmt)tabbar_read_vbo(win, sty, act, pin);
	else tabbar_read_pixel(win, sty, act, pin);
}




static int tabbar_swrite_child(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
    int x,y,j,k;
    short* t;
    struct relation* rel;
    struct arena* tmp;
    struct style* st;
    if(act->x0 < 0)return 0;

    j = 0;
    rel = win->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_win_ == rel->dsttype){
            tmp = (void*)(rel->dstchip);
            if(j == act->x0)goto found;

            j += 1;
            if(j >= 8)break;
        }

        rel = samesrcnextdst(rel);
    }
    return 0;

found:
    t = (void*)ev;
    x = t[0] * 16 / (win->width);
    y = t[1] * 16 / (win->height);
    if(x < 4)return 0;
    if(x >= 12)return 0;
    if(y < 4)return 0;
    if(y >= 12)return 0;

    j = 0;
    k = (y-4)*8 + (x-4);
    rel = tmp->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_act_ == rel->dsttype){
            st = (void*)(rel->srcfoot);
            if(j == k){
                if('#' == st->uc[3])st->uc[3] = 0;
                else st->uc[3] = '#';
                return 1;
            }

            j += 1;
            if(j >= 64)break;
        }

        rel = samesrcnextdst(rel);
    }
    return 1;
}
static int tabbar_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
    int x,y,ret;
    short* t;

    if(0x2d70 == ev->what)
    {
        ret = tabbar_swrite_child(act, pin, win, sty, ev, len);
        if(ret)return 1;

        t = (void*)ev;
        y = t[1] * 32 / (win->height);
        x = t[0] * 16 / (win->width);
        x -= 4;

        if( (y >= 31) && (x >= 0) && (x < 8) ){
            if(act->x0 == x)act->x0 = -1;
            else act->x0 = x;
            return 1;
        }else{
            act->x0 = -1;
        }
    }
    return 0;
}
static void tabbar_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void tabbar_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void tabbar_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void tabbar_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void tabbar_delete()
{
}
static void tabbar_create(struct actor* act, void* str)
{
    act->x0 = -1;
}




void tabbar_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'a', 'b', 'b', 'a', 'r', 0, 0);

	p->oncreate = (void*)tabbar_create;
	p->ondelete = (void*)tabbar_delete;
	p->onstart  = (void*)tabbar_start;
	p->onstop   = (void*)tabbar_stop;
	p->oncread  = (void*)tabbar_cread;
	p->oncwrite = (void*)tabbar_cwrite;
	p->onsread  = (void*)tabbar_sread;
	p->onswrite = (void*)tabbar_swrite;
}
