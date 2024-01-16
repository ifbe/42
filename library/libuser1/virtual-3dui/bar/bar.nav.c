#include "libuser.h"
int entityoutput_void(    _obj* win, struct style* sty);
int entityoutput_console( _obj* win, struct style* sty);
int entityoutput_overview(_obj* win, struct style* sty);
int entityoutput_detail(  _obj* win, struct style* sty);
int entityoutput_editor(  _obj* win, struct style* sty);
int entityoutput_player( _obj* win, struct style* sty);




static void* nametab[8] = {
	"test",
	"term",
	"wire",
	"node",
	"cad ",
	"rpg ",
	"rts ",
	"    "
};




void navmenu_draw_cli(_obj* win)
{
}
void navmenu_draw_tui(_obj* win)
{
}
void navmenu_draw_html(_obj* win)
{
}
void navmenu_draw_json(_obj* win)
{
}
void navmenu_draw_gl41(_obj* win)
{
	struct style sty;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	int j,k,x,y;
	int rgb,tmp;
	int w = win->whdf.width;
	int s = w*2/3;
/*
	j = win->forex;
	k = j;
	if(k < -s)k = -s;
	if(k > s)k = s;

	tmp = win->forew & 7;
	for(x=0;x<8;x++)
	{
		vc[0] = (x-tmp)*4.0/3 + (float)k*2.0/w;
		vc[1] = 0.0;
		vc[2] = -0.5;
		vr[0] = 0.5;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = 0.5;
		vf[2] = 0.0;
		if(x == tmp)
		{
			if(j<0)
			{
				vc[1] += (s+k)/12.0/s;
				vr[0] += (s+k)/12.0/s;
				vf[1] += (s+k)/12.0/s;
				rgb = 0x80*(s+k)/s;
				rgb = 0x404040 + (rgb<<16) + (rgb<<8) + rgb;
			}
			else
			{
				vc[1] += (s-k)/12.0/s;
				vr[0] += (s-k)/12.0/s;
				vf[1] += (s-k)/12.0/s;
				rgb = 0x80*(s-k)/s;
				rgb = 0x404040 + (rgb<<16) + (rgb<<8) + rgb;
			}
		}
		else
		{
			rgb = 0x404040;
			if((j<0)&&(x == tmp+1))
			{
				vc[1] -= k/12.0/s;
				vr[0] -= k/12.0/s;
				vf[1] -= k/12.0/s;
				rgb = 0x80*(-k)/s;
				rgb = 0x404040 + (rgb<<16) + (rgb<<8) + rgb;
			}
			if((j>0)&&(x+1 == tmp))
			{
				vc[1] += k/12.0/s;
				vr[0] += k/12.0/s;
				vf[1] += k/12.0/s;
				rgb = 0x80*k/s;
				rgb = 0x404040 + (rgb<<16) + (rgb<<8) + rgb;
			}
		}
		gl41solid2d_rect(win, rgb, vc, vr, vf);
		vc[2] = -0.51;
		gl41line2d_rect(win, 0xff00ff, vc, vr, vf);

		if((x >= tmp-1)&&(x <= tmp+1))
		{
			sty.vc[0] = vc[0];
			sty.vc[1] = vc[1];
			sty.vc[2] = vc[2];
			sty.vr[0] = vr[0];
			sty.vr[1] = vr[1];
			sty.vr[2] = vr[2];
			sty.vf[0] = vf[0];
			sty.vf[1] = vf[1];
			sty.vf[2] = vf[2];

			if(0 == x)entityoutput_void(win, &sty);
			else if(1 == x)entityoutput_console(win, &sty);
			//else if(2 == x)entityoutput_overview(win, &sty);
			else if(3 == x)entityoutput_detail(win, &sty);
		}
	}

	vc[0] = 0.0;
	vc[1] = 13.0/16;
	vc[2] = -0.5;
	vr[0] = 0.5;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 1.0/16;
	vf[2] = 0.0;
	gl41line2d_rect(win, 0xffffff, vc, vr, vf);
	vr[0] /= 4.0;
	gl41string2d_center(win, 0xffffff, vc, vr, vf, nametab[tmp], 0);
*/
}
void navmenu_draw_pixel(_obj* win)
{
	struct style sty;
	int va[2];
	int vb[2];
	int j,k,x,y;
	int tmp,rgb;
	int w = win->whdf.width;
	int h = win->whdf.height;
	int s = w*2/3;
/*
	j = win->forex;
	k = j;
	if(k < -s)k = -s;
	if(k > s)k = s;

	tmp = win->forew & 7;
	for(x=0;x<8;x++)
	{
		va[0] = w/2 - w/4 + k + (x-tmp)*s;
		vb[0] = w/2 + w/4 + k + (x-tmp)*s;
		va[1] = h/2 - h/4;
		vb[1] = h/2 + h/4;
		if(x == tmp)
		{
			if(j<0)
			{
				va[0] -= (s+k)*(w/24)/s;
				vb[0] += (s+k)*(w/24)/s;
				va[1] -= (s+k)*(h/12)/s;
				rgb = 0x80*(s+k)/s;
				rgb = 0x404040 + (rgb<<16) + (rgb<<8) + rgb;
			}
			else
			{
				va[0] -= (s-k)*(w/24)/s;
				vb[0] += (s-k)*(w/24)/s;
				va[1] -= (s-k)*(h/12)/s;
				rgb = 0x80*(s-k)/s;
				rgb = 0x404040 + (rgb<<16) + (rgb<<8) + rgb;
			}
		}
		else
		{
			rgb = 0x404040;
			if((j<0)&&(x == tmp+1))
			{
				va[0] += k*(w/24)/s;
				vb[0] -= k*(w/24)/s;
				va[1] += k*(h/12)/s;
				rgb = 0x80*(-k)/s;
				rgb = 0x404040 + (rgb<<16) + (rgb<<8) + rgb;
			}
			if((j>0)&&(x+1 == tmp))
			{
				va[0] -= k*(w/24)/s;
				vb[0] += k*(w/24)/s;
				va[1] -= k*(h/12)/s;
				rgb = 0x80*k/s;
				rgb = 0x404040 + (rgb<<16) + (rgb<<8) + rgb;
			}
		}

		drawsolid_rect(win, rgb, va[0], va[1], vb[0], vb[1]);
		drawline_rect(win, 0xff00ff, va[0], va[1], vb[0], vb[1]);

		if((x >= tmp-1)&&(x <= tmp+1))
		{
			sty.vc[0] = (va[0] + vb[0])/2;
			sty.vc[1] = (va[1] + vb[1])/2;
			sty.vc[2] = 0;
			sty.vr[0] = (vb[0] - va[0])/2;
			sty.vr[1] = 0;
			sty.vr[2] = 0;
			sty.vf[0] = 0;
			sty.vf[1] = (vb[1] - va[1])/2;

			if(0 == x)entityoutput_void(win, &sty);
			else if(1 == x)entityoutput_console(win, &sty);
			//else if(2 == x)entityoutput_overview(win, &sty);
			else if(3 == x)entityoutput_detail(win, &sty);
		}
	}

	drawline_rect(win, 0xffffff, w/4, h/16, w*3/4, h/8);
	drawstring_fit(win, 0xffffff, w/4, h/16, w*3/4, h/8, nametab[tmp], 0);
*/
}




int navmenu_event(_obj* win, struct style* sty, struct event* ev)
{
	short* t;
	int pa[2];
	int pb[2];
	int x, y, ret;
	ret = ev->what;
/*
	if('p' == (ret&0xff))
	{
		pb[0] = (ev->why)&0xffff;
		pb[1] = ((ev->why)>>16)&0xffff;
		if(win->input[10].z0)
		{
			pa[0] = win->input[10].x0;
			pa[1] = win->input[10].y0;
			x = pb[0] - pa[0];
			y = pb[1] - pa[1];
		}
		else if(win->input[0].z0)
		{
			pa[0] = win->input[0].x0;
			pa[1] = win->input[0].y0;
			x = pb[0] - pa[0];
			y = pb[1] - pa[1];
		}
		else x = y = 0;

		if(0x2d70 != ret)
		{
			win->forex = x;
			win->forey = y;
			return 0;
		}

		win->forex = 0;
		win->forey = 0;
		if((x>-16)&&(x<16)&&(y>-16)&&(y<16))
		{
			win->forew &= 0xf;
			return 0;
		}
		else if(x*8 < -win->whdf.width)x = 0x4d;
		else if(x*8 > win->whdf.width)x = 0x4b;
		else return 0;
	}
	else if(_kbd_ == ret)
	{
		x = ev->why;
	}
	else if(_char_ == ret)
	{
		ret = ev->why;
		logtoall("%x\n",ret);
		if(0x445b1b == ret)x = 0x4b;
		else if(0x435b1b == ret)x = 0x4d;
		else
		{
			if((0xd == ret)|(0xa == ret))win->forew &= 0xf;
			return 0;
		}
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		if(t[3] & joyl_left)x = 0x4b;
		else if(t[3] & joyl_right)x = 0x4d;
		else return 0;
	}
	else if(joy_right == (ev->what & joy_mask))
	{
		t = (void*)ev;
		if(t[3] & joyr_down)win->forew &= 0xf;
		return 0;
	}
	else return 0;

	//left
	if(0x4b == x)
	{
		x = win->forew & 7;
		x = (x+7)%8;
		y = win->forew & 0xf0;
		win->forew = y | x;
		return 0;
	}

	//right
	if(0x4d == x)
	{
		x = win->forew & 7;
		x = (x+1)%8;
		y = win->forew & 0xf0;
		win->forew = y | x;
		return 0;
	}*/
	return 0;
}
