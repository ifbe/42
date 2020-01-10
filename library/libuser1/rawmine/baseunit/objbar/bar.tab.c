#include "libuser.h"




void tabbar_gl41_listtwig(struct entity* win, struct style* sty, struct entity* tmp, int t)
{
    int x,y,j,rgb;
    vec3 rr;
    vec3 tc, tr, tf;
    struct relation* rel;
    struct entity* ac;
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
    gl41opaque_rect(win, 0x7f808080, tc, tr, tf);

    tc[0] = 0.0;
    tc[1] = 0.0;
    tc[2] = -0.8;
    tr[0] = 0.5;
    tr[1] = 0.0;
    tr[2] = 0.0;
    tf[0] = 0.0;
    tf[1] = 0.5;
    tf[2] = 0.0;
    gl41opaque_rect(win, 0x7f808080, tc, tr, tf);

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
        if(_ent_ == rel->dsttype){
            x = j%8;
            y = j/8;
            tc[0] = (2*x-7)/16.0;
            tc[1] = (7-2*y)/16.0;
            gl41opaque_rect(win, 0x7fffffff, tc, tr, tf);

            ac = (void*)(rel->dstchip);
            st = (void*)(rel->srcfoot);
            if('#' == st->i.uc[3])rgb = 0x404040;
            else rgb = 0xff00ff;
            carvestring_center(win, rgb, tc, rr, tf, (void*)&ac->fmt, 8);

            j++;
            if(j >= 64)break;
        }
        rel = samesrcnextdst(rel);
    }
}
void tabbar_gl41_listroot(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
    int j,rgb;
    vec3 pc,pf;
    vec3 tc,tr,tf;
    struct relation* rel;
    struct style* st;
    struct entity* aa;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

    tc[0] = vc[0] - vf[0]*31/32;
    tc[1] = vc[1] - vf[1]*31/32;
    tc[2] = vc[2] - vf[2]*31/32 - 0.5;
    tr[0] = vr[0] / 2;
    tr[1] = vr[1] / 2;
    tr[2] = vr[2] / 2;
    tf[0] = vf[0] / 32;
    tf[1] = vf[1] / 32;
    tf[2] = vf[2] / 32;
	gl41line_rect(win, 0xff0000, tc, tr, tf);

    j = 0;
    rel = win->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_sup_ == rel->dsttype){
            tc[0] = vc[0] + vr[0]*(2*j-7)/16 - vf[0]*31/32;
            tc[1] = vc[1] + vr[1]*(2*j-7)/16 - vf[1]*31/32;
            tc[2] = vc[2] + vr[2]*(2*j-7)/16 - vf[2]*31/32 - 0.8;
            tr[0] = vr[0] / 2 / 8.1;
            tr[1] = vr[1] / 2 / 8.1;
            tr[2] = vr[2] / 2 / 8.1;
            gl41opaque_rect(win, 0x7f000080, tc, tr, tf);

            tc[2] -= 0.1;
            tr[0] = vr[0] / 2 / 16;
            tr[1] = vr[1] / 2 / 16;
            tr[2] = vr[2] / 2 / 16;

            aa = (void*)(rel->dstchip);
            carvestring_center(win, 0xffffff, tc, tr, tf, (void*)(&aa->fmt), 8);

            if(j == act->ix0)tabbar_gl41_listtwig(win, sty, aa, j);

            j++;
            if(j == 8)break;
        }

        rel = samesrcnextdst(rel);
    }
}
void tabbar_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	tabbar_gl41_listroot(act, pin, win, sty);
}




void tabbar_pixel_listtwig(struct entity* win, struct style* sty, struct entity* tmp, int t)
{
    int x,y,j,rgb;
    struct relation* rel;
    struct entity* ac;
    struct style* st;
    int w = win->width;
    int h = win->height;

    drawsolid_rect(win, 0x808080, (t+4)*w/16, h*3/4, (t+5)*w/16, h*31/32);
    drawsolid_rect(win, 0x808080, w/4, h/4, w*3/4, h*3/4);

    j = 0;
    rel = tmp->orel0;
    while(1){
        if(0 == rel)break;
        if(_ent_ == rel->dsttype){
            x = j%8;
            y = j/8;
            drawsolid_rect(
                win, 0xffffff,
                (x+4)*w/16, (y+4)*h/16,
                (x+5)*w/16, (y+5)*h/16
            );

            ac = (void*)(rel->dstchip);
            st = (void*)(rel->srcfoot);
            if('#' == st->i.uc[3])rgb = 0x404040;
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
    int j,rgb;
    struct relation* rel;
    struct style* st;
    struct entity* aa;
    int w = win->width;
    int h = win->height;

    j = 0;
    rel = win->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_sup_ == rel->dsttype){
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

            if(j == act->ix0)tabbar_pixel_listtwig(win, sty, aa, j);

            j++;
            if(j == 8)break;
        }

        rel = samesrcnextdst(rel);
    }
}
void tabbar_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
    tabbar_pixel_listroot(act, pin, win, sty);
}




void tabbar_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
void tabbar_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
void tabbar_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
void tabbar_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static int tabbar_event_child(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
    int x,y,j,k;
    short* t;
    struct relation* rel;
    struct entity* tmp;
    struct style* st;
    if(act->ix0 < 0)return 0;

    j = 0;
    rel = win->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_sup_ == rel->dsttype){
            tmp = (void*)(rel->dstchip);
            if(j == act->ix0)goto found;

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

        if(_ent_ == rel->dsttype){
            st = (void*)(rel->srcfoot);
            if(j == k){
                if('#' == st->i.uc[3])st->i.uc[3] = 0;
                else st->i.uc[3] = '#';
                return 1;
            }

            j += 1;
            if(j >= 64)break;
        }

        rel = samesrcnextdst(rel);
    }
    return 1;
}
static int tabbar_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
    int x,y,ret;
    short* t;

    if(0x2d70 == ev->what)
    {
        ret = tabbar_event_child(act, pin, win, sty, ev, len);
        if(ret)return 1;

        t = (void*)ev;
        y = t[1] * 32 / (win->height);
        x = t[0] * 16 / (win->width);
        x -= 4;

        if( (y >= 31) && (x >= 0) && (x < 8) ){
            if(act->ix0 == x)act->ix0 = -1;
            else act->ix0 = x;
            return 1;
        }else{
            act->ix0 = -1;
        }
    }
    return 0;
}




static void tabbar_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//tabbar_draw(act, pin, win, sty);
}
static int tabbar_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return 0;//tabbar_event(act, pin, win, sty, ev, 0);
}
static void tabbar_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void tabbar_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void tabbar_search(struct entity* act)
{
}
static void tabbar_modify(struct entity* act)
{
}
static void tabbar_delete(struct entity* act)
{
}
static void tabbar_create(struct entity* act, void* str)
{
    act->ix0 = -1;
}




void tabbar_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'a', 'b', 'b', 'a', 'r', 0, 0);

	p->oncreate = (void*)tabbar_create;
	p->ondelete = (void*)tabbar_delete;
	p->onsearch = (void*)tabbar_search;
	p->onmodify = (void*)tabbar_modify;

	p->onlinkup = (void*)tabbar_linkup;
	p->ondiscon = (void*)tabbar_discon;
	p->onread  = (void*)tabbar_read;
	p->onwrite = (void*)tabbar_write;
}
