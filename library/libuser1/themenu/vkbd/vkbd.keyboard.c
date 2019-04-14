#include "libuser.h"




void vkbd_keyboard_read_pixel(struct arena* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,m,n;
	int w = win->width;
	int h = win->height;
	//if(win->vkbdw < 0)return;

    drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);

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

            drawicon_1(
                win, rgb,
                (x*w/16)+1, h+1-((y+1)*h/32)+1,
                ((x+1)*w/16)-1, h-1-(y*h/32),
                (u8*)&c, l
            );
        }
    }
}
void vkbd_keyboard_read_vbo(struct arena* win, struct style* sty)
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
			rgb = 0x808080;

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
void vkbd_keyboard_read_html(struct arena* win, struct style* sty)
{
}
void vkbd_keyboard_read_tui(struct arena* win, struct style* sty)
{
}
void vkbd_keyboard_read_cli(struct arena* win, struct style* sty)
{
}
void vkbd_keyboard_read(struct arena* cc, void* cf, struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)vkbd_keyboard_read_cli(win, sty);
	else if(fmt == _tui_)vkbd_keyboard_read_tui(win, sty);
	else if(fmt == _html_)vkbd_keyboard_read_html(win, sty);
	else if(fmt == _vbo_)vkbd_keyboard_read_vbo(win, sty);
	else vkbd_keyboard_read_pixel(win, sty);
}




int vkbd_keyboard_write(struct arena* win, struct event* ev)
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
		//ret = win->vkbdw;
		if('k' == ret)
		{
			x = x*16/w;
			y = 31 - (y*32/h);
			ret = x + (y*16);

			//win->vkbdz = ret;
			//win->vkbdw = 'k';
			eventwrite(ret, _char_, (u64)win, 0);
		}
		else if('j' == ret)
		{
			y = (h-y)*16/h;
			if(x*2 < w)
			{
				x = x*16/h;
				if((0==x)&&(1==y))ret = joyl_left;
				else if((2==x)&&(1==y))ret = joyl_right;
				else if((1==x)&&(0==y))ret = joyl_down;
				else if((1==x)&&(2==y))ret = joyl_up;
				else if((0==x)&&(3==y))ret = joyl_trigger;
				else if((2==x)&&(3==y))ret = joyl_bumper;
				else if((1==x)&&(1==y))ret = joyl_stick;
				else if((3==x)&&(2==y))ret = joyl_select;
				else ret = 0;

				if(ret)
				{
					tmp[0] = tmp[1] = tmp[2] = 0;
					tmp[3] = ret;
					eventwrite(*(u64*)tmp, joy_left, 0, 0);
				}
			}
			else
			{
				x = w-x;
				x = x*16/h;
				if((2==x)&&(y==1))ret = joyr_left;
				else if((0==x)&&(1==y))ret = joyr_right;
				else if((1==x)&&(y==0))ret = joyr_down;
				else if((1==x)&&(y==2))ret = joyr_up;
				else if((2==x)&&(y==3))ret = joyr_trigger;
				else if((0==x)&&(y==3))ret = joyr_bumper;
				else if((1==x)&&(y==1))ret = joyr_stick;
				else if((3==x)&&(y==2))ret = joyr_start;
				else ret = 0;

				if(ret)
				{
					tmp[0] = tmp[1] = tmp[2] = 0;
					tmp[3] = ret;
					eventwrite(*(u64*)tmp, joy_right, 0, 0);
				}
			}

			//win->vkbdz = ret;
			//win->vkbdw = 'j';
		}
	}

byebye:
	return 1;
}
