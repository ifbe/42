#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include"libuser.h"




void windowread(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct relation* rel = wnd->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dsttype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].flag = rel->dstflag;
			entityread(stack[sp+1].pchip, stack[sp+1].flag, stack,sp+2, arg,key, buf,len);
		}
		rel = samesrcnextdst(rel);
	}
}
void windowwrite(struct supply* win,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
}
void windowlist()
{
}
void windowchange()
{
}
void windowstart()
{
}
void windowstop()
{
}
void windowdelete(struct supply* w)
{
}
void windowcreate(struct supply* w)
{
	w->fmt = _cli_;
	w->vfmt = 0;

	w->width = 80;
	w->height = 25;

	w->fbwidth = 80;
	//w->fbheight = 0;
}




void initwindow()
{
}
void freewindow()
{
}
