#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include"libuser.h"




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct relation* rel = wnd->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dstnodetype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].foottype = rel->srcfoottype;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].foottype = rel->dstfoottype;
			entity_takeby(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
		}
		rel = samesrcnextdst(rel);
	}
}
void window_give(_obj* win,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
}




void windowdelete(_obj* w)
{
}
void windowcreate(_obj* w)
{
	w->hfmt = _cli_;
	w->vfmt = 0;

}




void initwindow()
{
}
void freewindow()
{
}
