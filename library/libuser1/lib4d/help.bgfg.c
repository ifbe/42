#include "actor.h"
void vkbd_read(void*);
void login_read(void*);




void background_cli(struct arena* win)
{
}
void background_tui(struct arena* win)
{
}
void background_html(struct arena* win)
{
	win->len = 0;
}
void background_vbo(struct arena* win)
{
	int j;
	float* f;
	float* p;
	struct texandobj* mod = win->mod;
	for(j=0;j<8;j++)
	{
		mod[j].ilen = 0;
		mod[j].vlen = 0;
	}
	for(j=0x80;j<0x88;j++)
	{
		mod[j].ilen = 0;
		mod[j].vlen = 0;
	}

	mod[4].vlen = 6;
	f = mod[4].vbuf;

	f[0] = -32.0;
	f[1] = 0.0;
	f[2] = 0.0;
	f[3] = 1.0;
	f[4] = 1.0;
	f[5] = 1.0;
	f[6] = 0.0;
	f[7] = 1.0;
	f[8] = 0.0;

	f[ 9] = 32.0;
	f[10] = 0.0;
	f[11] = 0.0;
	f[12] = 1.0;
	f[13] = 1.0;
	f[14] = 1.0;
	f[15] = 1.0;
	f[16] = 1.0;
	f[17] = 0.0;

	f[18] = -32.0;
	f[19] = 0.0;
	f[20] = 64.0;
	f[21] = 1.0;
	f[22] = 1.0;
	f[23] = 1.0;
	f[24] = 0.0;
	f[25] = 0.0;
	f[26] = 0.0;

	f[27] = 32.0;
	f[28] = 0.0;
	f[29] = 0.0;
	f[30] = 1.0;
	f[31] = 1.0;
	f[32] = 1.0;
	f[33] = 1.0;
	f[34] = 1.0;
	f[35] = 0.0;

	f[36] = -32.0;
	f[37] = 0.0;
	f[38] = 64.0;
	f[39] = 1.0;
	f[40] = 1.0;
	f[41] = 1.0;
	f[42] = 0.0;
	f[43] = 0.0;
	f[44] = 0.0;

	f[45] = 32.0;
	f[46] = 0.0;
	f[47] = 64.0;
	f[48] = 1.0;
	f[49] = 1.0;
	f[50] = 1.0;
	f[51] = 1.0;
	f[52] = 0.0;
	f[53] = 0.0;

}
void background_pixel(struct arena* win)
{
	int x;
	int len = (win->stride)*(win->height);
	u32* buf = (u32*)(win->buf);

	for(x=0;x<len;x++)buf[x] = 0xff000000;
}
void background(struct arena* win)
{
	u64 fmt = win->fmt;
	//if((_vbo_ != win->fmt) | (12 == win->flag0));

	if(_cli_ == fmt)background_cli(win);
	else if(_tui_ == fmt)background_tui(win);
	else if(_html_ == fmt)background_html(win);
	else if(_vbo_ == fmt)background_vbo(win);
	else background_pixel(win);
}




void foreground_cli(struct arena* win)
{
}
void foreground_tui(struct arena* win)
{
}
void foreground_html(struct arena* win)
{
}
void foreground_vbo(struct arena* win)
{
	int j;
	float x0,y0,x1,y1;
	for(j=0;j<11;j++)
	{
		if(0 == win->touchdown[j].z)continue;

		x0 = (float)(win->touchdown[j].x) / (float)(win->width);
		x0 = x0*2 - 1.0;
		y0 = (float)(win->touchdown[j].y) / (float)(win->height);
		y0 = 1.0 - y0*2;
		x1 = (float)(win->touchmove[j].x) / (float)(win->width);
		x1 = x1*2 - 1.0;
		y1 = (float)(win->touchmove[j].y) / (float)(win->height);
		y1 = 1.0 - y1*2;
		carveline2d(win, 0xff00ff, x0, y0, 0.0, x1, y1, 0.0);
	}
	vkbd_read(win);
}
void foreground_pixel(struct arena* win)
{
	int j;
	for(j=0;j<11;j++)
	{
		if(0 == win->touchdown[j].z)continue;

		drawline(win, 0xff00ff,
			win->touchdown[j].x, win->touchdown[j].y,
			win->touchmove[j].x, win->touchmove[j].y
		);
	}
	vkbd_read(win);
}
void foreground(struct arena* win)
{
	u64 fmt = win->fmt;
	if((win->theone) | (0 == win->irel))login_read(win);

	if(_cli_ == fmt)foreground_cli(win);
	else if(_tui_ == fmt)foreground_tui(win);
	else if(_html_ == fmt)foreground_html(win);
	else if(_vbo_ == fmt)foreground_vbo(win);
	else foreground_pixel(win);
}
