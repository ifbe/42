#include "libuser.h"
int vkbd_joystick_read(struct arena* cc, void* cf, struct arena* win, void* sty);
int vkbd_joystick_write(struct arena* win, struct event* ev);
int vkbd_keyboard_read(struct arena* cc, void* cf, struct arena* win, void* sty);
int vkbd_keyboard_write(struct arena* win, struct event* ev);



int vkbd_read(struct arena* cc, struct style* cf, struct arena* win, struct style* sty)
{
        switch(win->vkbdw)
        {
                case 'j':vkbd_joystick_read(0, 0, win, sty);break;
                case 'k':vkbd_keyboard_read(0, 0, win, sty);break;
        }
        return 0;
}
int vkbd_write(struct arena* win, struct event* ev)
{
        int j,k,x,y;
	int w,h,id,ret;
	u64 why = ev->why;
	u64 what = ev->what;
	if('p' == (what&0xff))
	{
		w = win->width;
		h = win->height;
		if(w<h)ret = w>>4;
		else ret = h>>4;

		id = (why>>48)&0xffff;
		if('l' == id)id = 10;
		if((0 != id)&&(10 != id))return 0;
		j = win->input[id].x0;
		k = win->input[id].y0;

		if(0x2d70 == what)
		{
			//open or close vkbd
			x = why&0xffff;
			y = (why>>16)&0xffff;
			if(y+ret > h)
			{
				if(x+ret > w)
				{
					if(win->vkbdw > 0)win->vkbdw = 0;
					else win->vkbdw = 'j';
					return 1;
				}
				else if(x < ret)
				{
					if(win->vkbdw > 0)win->vkbdw = 0;
					else win->vkbdw = 'k';
					return 1;
				}
			}
			if(k+ret > h)
			{
				if(j+ret > w)
				{
					if(x*2 < w)
					{
						j = win->forew;
						win->forew = (j & 0xf) | 0x10;
					}
					return 1;
				}
				else if(j < ret)
				{
					if(x*2 > w)
                                        {
						j = win->forew;
						win->forew = (j & 0xf) | 0x20;
					}
					return 1;
				}
			}
		}
		else if(0x4070 == what)
		{
			if(k+ret > h)
			{
				if(j+ret > w)return 1;
				if(j-ret < 0)return 1;
			}
		}

		//call vkbd
                if(0 == win->vkbdw)return 0;
		switch(win->vkbdw)
		{
			case 'j':ret = vkbd_joystick_write(win, ev);break;
			case 'k':ret = vkbd_keyboard_write(win, ev);break;
		}
		if(0 != ret)return 1;
	}
	return 0;
}




int vkbd_start(struct arena* c, void* cf, struct arena* r, void* rf)
{
        return 0;
}
int vkbd_create(struct arena* win, u8* str)
{
        return 0;
}