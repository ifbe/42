#include "libuser.h"




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
void tabbar_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
    int j,rgb;
    vec3 tc,tr,tf;
    struct arena* tmp;
    struct style* st;
    struct actor* ac;
    struct relation* rel;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
    //carvesolid2d_rect(win, 0xff0000, vc, vr, vf);

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

    rel = act->irel0;
    if(0 == rel)return;

    tmp = (void*)(rel->srcchip);
    if(0 == tmp)return;

    j = 0;
    rel = tmp->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_act_ == rel->dsttype)
        {
            tc[0] = vc[0] + vr[0]*(2*j-7)/16 - vf[0]*31/32;
            tc[1] = vc[1] + vr[1]*(2*j-7)/16 - vf[1]*31/32;
            tc[2] = vc[2] + vr[2]*(2*j-7)/16 - vf[2]*31/32 - 0.5;
            tr[0] = vr[0] / 2 / 8.1;
            tr[1] = vr[1] / 2 / 8.1;
            tr[2] = vr[2] / 2 / 8.1;
            carvesolid2d_rect(win, 0x000080, tc, tr, tf);

            ac = (void*)(rel->dstchip);
            st = (void*)(rel->srcfoot);
            if('#' == st->uc[3])rgb = 0x111111;
            else rgb = 0xffffff;

            tc[2] -= 0.1;
            tr[0] = vr[0] / 2 / 16;
            tr[1] = vr[1] / 2 / 16;
            tr[2] = vr[2] / 2 / 16;
            carvestring2d_center(win, rgb, tc, tr, tf, (void*)(&ac->fmt), 8);

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
    int j,rgb;
    struct relation* rel;
    struct arena* tmp;
    struct style* st;
    struct actor* ac;
    int w = win->width;
    int h = win->height;
	drawline_rect(win, 0xff0000, w/4, h*31/32, w*3/4, h-1);

    rel = act->irel0;
    if(0 == rel)return;

    tmp = (void*)(rel->srcchip);
    if(0 == tmp)return;

    j = 0;
    rel = tmp->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_act_ == rel->dsttype)
        {
            drawsolid_rect(win, 0x000080, w*(j+4)/16+2, h*31/32+2, w*(j+5)/16-2, h-2);

            ac = (void*)(rel->dstchip);
            st = (void*)(rel->srcfoot);
            if('#' == st->uc[3])rgb = 0x111111;
            else rgb = 0xffffff;

            drawstring_fit(win, rgb, w*(j+4)/16, h*31/32, w*(j+5)/16, h, (void*)(&ac->fmt), 8);

            j++;
            if(j == 8)break;
        }

        rel = samesrcnextdst(rel);
    }
}
static void tabbar_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(_cli_ == fmt)tabbar_read_cli(win, sty, act, pin);
	else if(_tui_ == fmt)tabbar_read_tui(win, sty, act, pin);
	else if(_html_ == fmt)tabbar_read_html(win, sty, act, pin);
	else if(_json_ == fmt)tabbar_read_json(win, sty, act, pin);
	else if(_vbo_ == fmt)tabbar_read_vbo(win, sty, act, pin);
	else tabbar_read_pixel(win, sty, act, pin);
}
static int tabbar_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
    int x,y;
    struct relation* rel;
    struct arena* tmp;
    struct style* st;
    int w = win->width;
    int h = win->height;

    if(0x2d70 == ev->what)
    {
        x = (ev->why)&0xffff;
        x = x*16/w;
        y = ((ev->why)>>16)&0xffff;
        y = y*32/h;
        if((y == 31) && (x >= 4) && (x < 12))
        {
            say("x=%x\n",x);

            rel = act->irel0;
            if(0 == rel)return 1;

            tmp = (void*)(rel->srcchip);
            if(0 == tmp)return 1;

            x -= 4;
            rel = tmp->orel0;
            while(1)
            {
                if(0 == rel)break;

                if(_act_ == rel->dsttype)
                {
                    if(0 == x)
                    {
                        st = (void*)(rel->srcfoot);
                        if('#' == st->uc[3])st->uc[3] = 0;
                        else st->uc[3] = '#';
                    }
                    x--;
                }

                rel = samesrcnextdst(rel);
            }
        }
    }
    return 0;
}
static void tabbar_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void tabbar_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
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
    say("@tabbar_start\n");
}
static void tabbar_delete()
{
}
static void tabbar_create(void* addr)
{
    say("@tabbar_create\n");
}




void tabbar_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'a', 'b', 'b', 'a', 'r', 0, 0);

	p->oncreate = (void*)tabbar_create;
	p->ondelete = (void*)tabbar_delete;
	p->onstart  = (void*)tabbar_start;
	p->onstop   = (void*)tabbar_stop;
	p->onget    = (void*)tabbar_cread;
	p->onpost   = (void*)tabbar_cwrite;
	p->onread   = (void*)tabbar_sread;
	p->onwrite  = (void*)tabbar_swrite;
}