#include "libuser.h"
int actorinput_editor_camera(struct arena* win, struct event* ev);




void actoroutput_player_vbo(struct arena* win, struct style* sty)
{
	int j;
	vec3 tc, tr, tf;

	//rpg.hp.bg
	tc[0] = -0.75;
	tc[1] = 1.0-1.0/16;
	tc[2] = 0.0;
	tr[0] = 0.25;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0/16;
	tf[2] = 0.0;
	carvesolid2d_rect(win, 0xff0000, tc, tr, tf);

	//rpg.mp.bg
	tc[0] = -tc[0];
	carvesolid2d_rect(win, 0x0000ff, tc, tr, tf);

	//rpg.hp.fg
	tc[0] = 0.125-1.0;
	tc[1] = 1.0-0.125;
	tc[2] = -0.5;
	tr[0] = 0.125;
	tf[1] = 0.125;
	carvesolid2d_circle(win, 0x800000, tc, tr, tf);

	//rpg.mp.fg
	tc[0] = -tc[0];
	carvesolid2d_circle(win, 0x000080, tc, tr, tf);
}
void actoroutput_player_pixel(struct arena* win, struct style* sty)
{
	int w = win->width;
	int h = win->height;

	//rpg.hp.bg
	drawsolid_rect(win, 0xff0000, 0, 0, w/4, h/16);

	//rpg.mp.bg
	drawsolid_rect(win, 0x0000ff, w*3/4, 0, w, h/16);

	//rpg.hp.fg
	drawsolid_circle(win, 0x800000, h/16, h/16, h/16);

	//rpg.mp.fg
	drawsolid_circle(win, 0x000080, w-(h/16), h/16, h/16);
}




int actoroutput_player_recursion(
	struct arena* uwin, struct style* usty,
	struct actor* uact, struct pinid* upin)
{
	int x,y,j;
	struct style st;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* orel;
	st.vr[0] = usty->vr[0]/2;
	st.vr[1] = usty->vr[1]/2;
	st.vr[2] = usty->vr[2]/2;
	st.vf[0] = usty->vf[0]/2;
	st.vf[1] = usty->vf[1]/2;
	st.vf[2] = usty->vf[2]/2;
	st.vu[0] = usty->vu[0]/2;
	st.vu[1] = usty->vu[1]/2;
	st.vu[2] = usty->vu[2]/2;

	j = 0;
	orel = uact->orel0;
	while(1)
	{
		if(0 == orel)break;

		if(_act_ == orel->dsttype)
		{
			x = j % 2;
			y = j / 2;
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			sty = (void*)(orel->srcfoot);
			if(sty)
			{
			}
			else
			{
				st.vc[0] = usty->vc[0] + st.vr[0]*(2*x-1) + st.vf[0]*(2*y-1);
				st.vc[1] = usty->vc[1] + st.vr[1]*(2*x-1) + st.vf[1]*(2*y-1);
				st.vc[2] = usty->vc[2] + st.vr[2]*(2*x-1) + st.vf[2]*(2*y-1);

				sty = &st;
			}

			act->onread(uwin, sty, act, pin);
			if(act->orel0)actoroutput_player_recursion(uwin, sty, act, pin);
		}

		j++;
		orel = samesrcnextdst(orel);
	}
	return 0;
}
int actorinput_player_recursion(
	struct arena* uwin, struct style* usty,
	struct actor* uact, struct pinid* upin)
{
	return 0;
}
int actoroutput_3d(struct arena* win, struct style* stack)
{
	struct relation* orel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	sty = 0;
	orel = win->orel0;
	while(1)
	{
		if(orel == 0)break;

		if(_act_ == orel->dsttype)
		{
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			//say("%x,%x,%x,%x\n", win, act, sty, pin);
			//say("%x\n", rel);

			sty = (void*)(orel->srcfoot);
			act->onread(win, sty, act, pin);
			if(act->orel0)actoroutput_player_recursion(win, sty, act, pin);
		}

		orel = samesrcnextdst(orel);
	}

    if(_vbo_ == win->fmt)
	{
		//if(sty)select_3d(win, 0x004000, sty, 0);
		actoroutput_player_vbo(win, stack);
	}
    else
	{
		//if(sty)select_2d(win, 0x004000, sty, 0);
		actoroutput_player_pixel(win, stack);
	}
	return 0;
}




int actorinput_rts(struct arena* win, struct style* sty, struct event* ev)
{
    float x,y;
    float dx,dy,dz,len;
    float* tar;
    float* cam;
    float* tmp;

    if('p' == (ev->what&0xff))
    {
        tar = win->target.vc;
        cam = win->camera.vc;
        tmp = win->camera.vf;

        x = (ev->why)&0xffff;
        y = ((ev->why)>>16)&0xffff;
        //say("x=%f,y=%f\n",x,y);

        if(x <= 9){tar[0] -= 9.0;cam[0] -= 9.0;}
        else if(x+9 >= win->width){tar[0] += 9.0;cam[0] += 9.0;}
        else if(y <= 9){tar[1] += 9.0;cam[1] += 9.0;}
        else if(y+9 >= win->height){tar[1] -= 9.0;cam[1] -= 9.0;}
        else return 0;

        dx = tar[0] - cam[0];
        dy = tar[1] - cam[1];
        dz = tar[2] - cam[2];
        len = squareroot(dx*dx + dy*dy + dz*dz) * 0.70710678118655;

        cam[0] = tar[0];
        cam[1] = tar[1] - len;
        cam[2] = tar[2] + len;
        tmp[0] = 0;
        tmp[1] = len;
        tmp[2] = -len;
    }
    return 0;
}
int actorinput_3d(struct arena* win, struct style* sty, struct event* ev)
{
	struct actor* act;
	struct pinid* pin;
	struct relation* orel;
    if(	('p' == (ev->what&0xff)) |
		(joy_event == (ev->what&joy_mask)) )
	{
		return actorinput_editor_camera(win, ev);
	}

	orel = win->oreln;
	if(0 == orel)return 0;

	act = (void*)(orel->dstchip);
	pin = (void*)(orel->dstfoot);
	return act->onwrite(act, pin, 0, 0, ev, 0);
}