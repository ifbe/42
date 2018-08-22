#include "libuser.h"
int actoroutput_console( struct arena* win, struct style* sty);
int actoroutput_overview(struct arena* win, struct style* sty);
int actoroutput_detail(  struct arena* win, struct style* sty);
int actoroutput_edit(    struct arena* win, struct style* sty);
int actoroutput_posture( struct arena* win, struct style* sty);
int actoroutput_deliver( struct arena* win, struct style* sty);
int actoroutput_oneonone(struct arena* win, struct style* sty);




static void* nametab[8] = {
	"  0: testland   ",
	"  1: console    ",
	"  2: overview   ",
	"  3: detail     ",
	"  4: layout     ",
	"  5: freeview   ",
	"  6: playgame   ",
	"  7: onetoone   "
};




void actoroutput_navmenu_cli(struct arena* win)
{
}
void actoroutput_navmenu_tui(struct arena* win)
{
}
void actoroutput_navmenu_html(struct arena* win)
{
}
void actoroutput_navmenu_json(struct arena* win)
{
}
void actoroutput_navmenu_vbo(struct arena* win)
{
	struct style sty;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	int j,k,x,y;
	int rgb,tmp;
	int w = win->width;
	int s = w*2/3;

	j = win->menux;
	k = j;
	if(k < -s)k = -s;
	if(k > s)k = s;

	tmp = win->modetype&7;
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
		carvesolid2d_rect(win, rgb, vc, vr, vf);
		vc[2] = -0.51;
		carveline2d_rect(win, 0xff00ff, vc, vr, vf);

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

			if(1 == x)actoroutput_console(win, &sty);
			else if(2 == x)actoroutput_overview(win, &sty);
			else if(3 == x)actoroutput_detail(win, &sty);
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
	carveline2d_rect(win, 0xffffff, vc, vr, vf);
	vr[0] /= 4.0;
	carvestring2d_center(win, 0xffffff, vc, vr, vf, nametab[tmp], 16);
/*
	vc[0] = 0.0;
	vc[1] = -1.0;
	vc[2] = -0.2;
	vr[0] = 0.0;
	vr[1] = 1.0;
	vr[2] = -0.2;
	carveline2d(win, 0xffffff, vc, vr);
	for(j=-3;j<4;j+=2)
	{
		vc[0] = -0.5;
		vc[1] = 0.25*j;
		vc[2] = -0.2;
		vr[0] = 0.5;
		vr[1] = 0.25*j;
		vr[2] = -0.2;
		carveline2d(win, 0xffffff, vc, vr);
	}

	k = win->menutype & 7;
	for(j=0;j<8;j++)
	{
		x = j%2;
		y = j/2;
		if(j == k)rgb = 0xff00ff;
		else rgb = 0x404040;

		vc[0] = x-0.5;
		vc[1] = (3-2*y)*0.25;
		vc[2] = -0.4;
		vr[0] = 0.4;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = 0.2;
		vf[2] = 0.0;
		carvesolid2d_rect(win, rgb, vc, vr, vf);

		vc[1] = (13-8*y)/16.0;
		vc[2] = -0.6;
		vr[0] = 1.0/8;
		vf[1] = 1.0/16;
		carvestring2d_center(win, ~rgb, vc, vr, vf, (u8*)name[y], 0);

		vc[1] = (11-8*y)/16.0;
		carvestring2d_center(win, ~rgb, vc, vr, vf, (u8*)target[x], 0);
	}
*/
}
void actoroutput_navmenu_pixel(struct arena* win)
{
	struct style sty;
	int va[2];
	int vb[2];
	int j,k,x,y;
	int tmp,rgb;
	int w = win->width;
	int h = win->height;
	int s = w*2/3;
/*
	drawline(win, 0xffffff, win->width/2, 0, win->width/2, win->height);
	for(j=1;j<8;j+=2)
	{
		va[0] = win->width/4;
		va[1] = win->height*j/8;
		vb[0] = win->width*3/4;
		vb[1] = win->height*j/8;
		drawline(win, 0xffffff, va[0], va[1], vb[0], vb[1]);
	}
*/
	j = win->menux;
	k = j;
	if(k < -s)k = -s;
	if(k > s)k = s;

	tmp = win->modetype&7;
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
/*
		drawline(win, 0xffffff, va[0], va[1], 0, 0);
		drawline(win, 0xffffff, va[0], va[1], w, 0);
		drawline(win, 0xffffff, vb[0], va[1], 0, 0);
		drawline(win, 0xffffff, vb[0], va[1], w, 0);
		drawline(win, 0xffffff, va[0], vb[1], 0, h);
		drawline(win, 0xffffff, va[0], vb[1], w, h);
		drawline(win, 0xffffff, vb[0], vb[1], 0, h);
		drawline(win, 0xffffff, vb[0], vb[1], w, h);
*/
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

			if(1 == x)actoroutput_console(win, &sty);
			else if(2 == x)actoroutput_overview(win, &sty);
			else if(3 == x)actoroutput_detail(win, &sty);
		}
	}

	drawline_rect(win, 0xffffff, w/4, h/16, w*3/4, h/8);
	drawstring_fit(win, 0xffffff, w/4, h/16, w*3/4, h/8, nametab[tmp], 16);
/*
	k = win->menutype & 7;
	for(j=0;j<8;j++)
	{
		x = j%2;
		y = j/2;
		va[0] = (win->width)*(8*x+0)/16;
		va[1] = (win->height)*(8*y+0)/32;
		vb[0] = (win->width)*(8*x+8)/16;
		vb[1] = (win->height)*(8*y+8)/32;

		if(j == k)rgb = 0xff00ff;
		else rgb = 0x404040;
		drawsolid_rect(win, rgb, va[0]+4, va[1]+4, vb[0]-4, vb[1]-4);
		drawline_rect(win, 0x808080, va[0]+4, va[1]+4, vb[0]-4, vb[1]-4);

		drawstring_fit(win, ~rgb, va[0], va[1], vb[0], (va[1]+vb[1])/2, (u8*)name[y], 0);
		drawstring_fit(win, ~rgb, va[0], (va[1]+vb[1])/2, vb[0], vb[1], (u8*)target[x], 0);
	}
*/
}
void actoroutput_navmenu(struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)actoroutput_navmenu_cli(win);
	else if(_tui_ == fmt)actoroutput_navmenu_tui(win);
	else if(_html_ == fmt)actoroutput_navmenu_html(win);
	else if(_json_ == fmt)actoroutput_navmenu_json(win);
	else if(_vbo_ == fmt)actoroutput_navmenu_vbo(win);
	else actoroutput_navmenu_pixel(win);
}
int actorinput_navmenu(struct arena* win, struct event* ev)
{
	short* t;
	int pa[2];
	int pb[2];
	int x, y, ret;
	ret = ev->what;

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
			win->menux = x;
			win->menuy = y;
			return 0;
		}

		win->menux = 0;
		win->menuy = 0;
		if((x>-16)&&(x<16)&&(y>-16)&&(y<16))
		{
			win->menutype = 0;
			return 0;
		}
		else if(x*8 < -win->width)x = 0x4d;
		else if(x*8 > win->width)x = 0x4b;
		else return 0;
	}
	else if(_kbd_ == ret)
	{
		x = ev->why;
	}
	else if(_char_ == ret)
	{
		ret = ev->why;
		say("%x\n",ret);
		if(0x445b1b == ret)x = 0x4b;
		else if(0x435b1b == ret)x = 0x4d;
		else
		{
			if((0xd == ret)|(0xa == ret))win->menutype = 0;
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
		if(t[3] & joyr_down)win->menutype = 0;
		return 0;
	}
	else return 0;

	//left
	if(0x4b == x)
	{
		y = win->modetype & 7;
		win->modetype = (y+7)%8;
		return 0;
	}

	//right
	if(0x4d == x)
	{
		y = win->modetype & 7;
		win->modetype = (y+1)%8;
		return 0;
	}
	return 0;
}
