#include "actor.h"




void touch_explain(struct arena* win, struct event* ev)
{
	int x,y,z,btn;
	if('p' != (ev->what & 0xff))return;

	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	z = ((ev->why)>>32)&0xffff;
	btn = ((ev->why)>>48)&0xffff;

	if('l' == btn)btn = 10;
	else if('r' == btn)btn = 11;
	else if(10 < btn)return;

	if(hex32('p','@',0,0) == ev->what)
	{
		win->touchmove[btn].x = x;
		win->touchmove[btn].y = y;
	}
	else if(hex32('p','+',0,0) == ev->what)
	{
		win->touchdown[btn].x = win->touchmove[btn].x = x;
		win->touchdown[btn].y = win->touchmove[btn].y = y;
		win->touchdown[btn].z = 1;
	}
	else if(hex32('p','-',0,0) == ev->what)
	{
		win->touchdown[btn].z = 0;
	}
}