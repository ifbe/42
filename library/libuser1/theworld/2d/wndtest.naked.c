#include "libuser.h"




int rgbanode_draw(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//pixel_clearcolor(wnd);

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
			entity_take(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
		}
next:
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int rgbanode_take(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return 0;
	rgbanode_draw(wnd,foot, stack,sp, arg,key, buf,len);
/*
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//slot type known: do work based on slot type
	//switch(stack[sp-1].flag){
	//}

	//slot type unknown: do work based on caller fmt
	switch(caller->hfmt){
	case _gl41list_:
		break;
	default:
		break;
	}
*/
	return 0;
}
int rgbanode_give(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@rgbanode_write:%p,%x\n", wnd,foot);
	//printmemory(buf,16);

	float max = -1.0;
	struct relation* rel = wnd->oreln;
	struct relation* the = rel;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
			struct style* sty = (void*)(rel->srcfoot);
			if(sty){
				//say("%f\n", sty->fs.vc[3]);
				if(max < sty->fs.vc[3]){
					the = rel;
					max = sty->fs.vc[3];
				}
			}
		}

		rel = samesrcprevdst(rel);
	}

//say("max=%f,the=%p\n",max,the);
	if(the){
		stack[sp+0].pchip = the->psrcchip;
		stack[sp+0].pfoot = the->psrcfoot;
		stack[sp+0].flag = the->srcflag;
		stack[sp+1].pchip = the->pdstchip;
		stack[sp+1].pfoot = the->pdstfoot;
		stack[sp+1].flag = the->dstflag;
		entity_give(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
	}
	return 0;
}
int rgbanode_discon(_obj* win, struct style* sty)
{
	return 0;
}
int rgbanode_linkup(_obj* twig, void* tf, _obj* root, void* rf)
{
	return 0;
}





int rgbanode_search(_obj* win)
{
	return 0;
}
int rgbanode_modify(_obj* win)
{
	return 0;
}
int rgbanode_delete(_obj* win)
{
	return 0;
}
int rgbanode_create(u64 type, void* addr)
{
	return 0;
}
