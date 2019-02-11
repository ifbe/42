#include "libuser.h"
int actorinput_void(     struct arena* win, struct style* sty, struct event* ev);
int actoroutput_void(    struct arena* win, struct style* sty);
int actorinput_console(  struct arena* win, struct style* sty, struct event* ev);
int actoroutput_console( struct arena* win, struct style* sty);
int actorinput_overview( struct arena* win, struct style* sty, struct event* ev);
int actoroutput_overview(struct arena* win, struct style* sty);
int actorinput_detail(   struct arena* win, struct style* sty, struct event* ev);
int actoroutput_detail(  struct arena* win, struct style* sty);
int actorinput_win(      struct arena* win, struct style* sty, struct event* ev);
int actoroutput_win(     struct arena* win, struct style* sty);
int actorinput_2d(       struct arena* win, struct style* sty, struct event* ev);
int actoroutput_2d(      struct arena* win, struct style* sty);
int actorinput_cad(      struct arena* win, struct style* sty, struct event* ev);
int actoroutput_cad(     struct arena* win, struct style* sty);
int actorinput_3d(       struct arena* win, struct style* sty, struct event* ev);
int actoroutput_3d(      struct arena* win, struct style* sty);




static void* nametab[8] = {
	"test",
	"term",
	"wire",
	"node",
	"win ",
	" 2d ",
	"cad ",
	" 3d "
};
void tabbar_actors(struct arena* win, struct style* sty)
{/*
    int sel = (win->forew)&0x7;
    if(0 == sel)actoroutput_void(win, sty);
    else if(1 == sel)actoroutput_console(win, sty);
    else if(2 == sel)actoroutput_overview(win, sty);
    else if(3 == sel)actoroutput_detail(win, sty);
    else if(4 == sel)actoroutput_win(win, sty);
    else if(5 == sel)actoroutput_2d(win, sty);
    else if(6 == sel)actoroutput_cad(win, sty);
    else if(7 == sel)actoroutput_3d(win, sty);*/
}




void actoroutput_tabbar_cli(struct arena* win)
{
}
void actoroutput_tabbar_tui(struct arena* win)
{
}
void actoroutput_tabbar_html(struct arena* win)
{
}
void actoroutput_tabbar_json(struct arena* win)
{
}
void actoroutput_tabbar_vbo(struct arena* win, struct style* sty)
{
    int j,c,sel;
    vec3 vc,vr,vf;
    struct style tmp;
    if(0 == sty)
    {
        sty = &tmp;
        sty->vc[0] = 0.0;
        sty->vc[1] = 1.0/20;
        sty->vc[2] = 0.0;
        sty->vr[0] = 1.0;
        sty->vr[1] = 0.0;
        sty->vr[2] = 0.0;
        sty->vf[0] = 0.0;
        sty->vf[1] = 19/20.0;
        sty->vf[2] = 0.0;
    }
    tabbar_actors(win, sty);

    sel = (win->forew)&0x7;
    vr[1] = 0.0;
    vr[2] = 0.0;
    vf[0] = 0.0;
    vf[1] = 1.0/20;
    vf[2] = 0.0;
    for(j=0;j<8;j++)
    {
        c = 0x404040;
        if(j == sel)c = 0xc0c0c0;

        vc[0] = (2*j-7) / 8.0;
        vc[1] = -1.0 + vf[1];
        vc[2] = -0.8;
        vr[0] = 1.0/8.5;
        carvesolid2d_rect(win, c, vc, vr, vf);
        carveline2d_rect(win, 0xffffff, vc, vr, vf);

        vc[2] = -0.9;
        vr[0] = 1.0/16;
        carvestring2d_center(win, 0xffffff, vc, vr, vf, nametab[j], 0);
    }
}
void actoroutput_tabbar_pixel(struct arena* win, struct style* sty)
{
    int w,h;
    int j,c,sel;
    struct style tmp;
    w = win->width;
    h = win->height;
    if(0 == sty)
    {
        sty = &tmp;
        sty->vc[0] = w/2;
        sty->vc[1] = h*19/40;
        sty->vc[2] = 0.0;
        sty->vr[0] = w/2;
        sty->vr[1] = 0.0;
        sty->vr[2] = 0.0;
        sty->vf[0] = 0.0;
        sty->vf[1] = h*19/40;
        sty->vf[2] = 0.0;
    }
    tabbar_actors(win, sty);

    sel = (win->forew)&0x7;
    for(j=0;j<8;j++)
    {
        c = 0x404040;
        if(j == sel)c = 0xc0c0c0;
        drawsolid_rect(win, c, j*w/8+2, h*19/20, (j+1)*w/8-2, h);
        drawline_rect(win, 0xffffff, j*w/8+2, h*19/20, (j+1)*w/8-2, h);
        drawstring_fit(win, 0xffffff, j*w/8, h*19/20, (j+1)*w/8, h, nametab[j], 0);
    }
}
void actoroutput_tabbar(struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(_cli_ == fmt)actoroutput_tabbar_cli(win);
	else if(_tui_ == fmt)actoroutput_tabbar_tui(win);
	else if(_html_ == fmt)actoroutput_tabbar_html(win);
	else if(_json_ == fmt)actoroutput_tabbar_json(win);
	else if(_vbo_ == fmt)actoroutput_tabbar_vbo(win, sty);
	else actoroutput_tabbar_pixel(win, sty);
}




int actorinput_tabbar(struct arena* win, struct style* sty, struct event* ev)
{
    int x,y,t;
    struct style tmp;
    int w = win->width;
    int h = win->height;
    int sel = (win->forew)&0x7;
    if(0 == sty)
    {
        sty = &tmp;
        sty->vc[0] = w/2;
        sty->vc[1] = h*19/40;
        sty->vc[2] = 0.0;
        sty->vr[0] = w/2;
        sty->vr[1] = 0.0;
        sty->vr[2] = 0.0;
        sty->vf[0] = 0.0;
        sty->vf[1] = h*19/40;
        sty->vf[2] = 0.0;
    }

    if(0x2d70 == ev->what)
    {
        y = ((ev->why)>>16)&0xffff;
        if(y > h*19/20)
        {
            x = (ev->why)&0xffff;
            x = x * 8 / w;

            t = win->forew;
            win->forew = (t & 0xf0) | x;

            if(_vbo_ == win->fmt)
            {
                if((4==x)|(5==x))win->vfmt = _2d_;
                else win->vfmt = _3d_;
            }
            return 1;
        }
    }
/*
    if(0 == sel)actorinput_void(         win, sty, ev);
    else if(1 == sel)actorinput_console( win, sty, ev);
    else if(2 == sel)actorinput_overview(win, sty, ev);
    else if(3 == sel)actorinput_detail(  win, sty, ev);
    else if(4 == sel)actorinput_win(     win, sty, ev);
    else if(5 == sel)actorinput_2d(      win, sty, ev);
    else if(6 == sel)actorinput_cad(     win, sty, ev);
    else if(7 == sel)actorinput_3d(      win, sty, ev);*/
    return 0;
}
