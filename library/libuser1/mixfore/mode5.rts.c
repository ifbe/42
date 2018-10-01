#include "libuser.h"




void actoroutput_rts_vbo(struct arena* win, struct style* sty)
{
    vec3 vc,vr,vf;

    vc[0] = 0.0;
    vc[1] = -5.0/6;
    vc[2] = -0.5;
    vr[0] = 0.5;
    vr[1] = 0.0;
    vr[2] = 0.0;
    vf[0] = 0.0;
    vf[1] = 1.0/6;
    vf[2] = 0.0;
    carvesolid2d_rect(win, 0x804000, vc, vr, vf);

    vc[0] = -0.75;
    vc[1] = -3.0/4;
    vc[2] = -0.5;
    vr[0] = 0.25;
    vr[1] = 0.0;
    vr[2] = 0.0;
    vf[0] = 0.0;
    vf[1] = 1.0/4;
    vf[2] = 0.0;
    carvesolid2d_rect(win, 0x004080, vc, vr, vf);

    vc[0] = 0.75;
    carvesolid2d_rect(win, 0x004080, vc, vr, vf);
}
void actoroutput_rts_pixel(struct arena* win, struct style* sty)
{
    int w = win->width;
    int h = win->height;
    drawsolid_rect(win, 0x004080, w*0/4, h*3/4, w/4, h);
    drawsolid_rect(win, 0x804000, w*1/4, h*5/6, w*3/4, h);
    drawsolid_rect(win, 0x004080, w*3/4, h*3/4, w, h);
}
int actoroutput_rts(struct arena* win, struct style* stack)
{
	struct relation* orel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;

	//world
	orel = win->orel0;
	while(1)
	{
		if(orel == 0)break;

		if(_act_ == orel->dsttype)
		{
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			sty = (void*)(orel->srcfoot);
			act->onread(win, sty, act, pin);
		}

		orel = samesrcnextdst(orel);
	}

	if(_vbo_ == win->fmt)actoroutput_rts_vbo(win, stack);
	else actoroutput_rts_pixel(win, stack);
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
