#include "libuser.h"
int gl41cmdq_clear(void*);




int wndmgr_mouseat_title(int x,int y, int x0,int y0, int xn,int yn)
{
	if(x < x0)return 0;
	if(x > xn)return 0;
	if(y < y0)return 0;
	if(y > yn)return 0;
	return 1;
}
int wndmgr_mouseat_border(int x,int y, int x0,int y0, int xn,int yn)
{
	if(x < x0-2)return 0;
	if(x > xn+2)return 0;
	if(y < y0-2)return 0;
	if(y > yn+2)return 0;

	if(x <= x0+2)return 1;
	if(x >= xn-2)return 1;
	if(y <= y0+2)return 1;
	if(y >= yn-2)return 1;

	return 0;
}
void* wndmgr_find_maxw(_obj* wnd)
{
	float max = -1.0;
	struct style* sty = 0;
	struct relation* rel = wnd->oreln;
	struct relation* the = rel;

	//find max_w
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
			sty = (void*)(rel->srcfoot);
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
	return the;
}
void* wndmgr_find_hit(_obj* wnd, int x, int y)
{
	struct style* sty = 0;
	struct relation* rel = wnd->oreln;

	//find max_w
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
			sty = (void*)(rel->srcfoot);
			if(sty){
				//say("%d,%d, %f,%f,%f,%f\n",x,y,sty->fs.vc[0],sty->fs.vc[1],sty->fs.vr[0],sty->fs.vf[1]);
				if(	(x > sty->fs.vc[0] - sty->fs.vr[0]) &&
					(x < sty->fs.vc[0] + sty->fs.vr[0]) &&
					(y > sty->fs.vc[1] - sty->fs.vf[1]) &&
					(y < sty->fs.vc[1] + sty->fs.vf[1]) )
				{
					return rel;
				}
			}
		}

		rel = samesrcprevdst(rel);
	}
	return rel;
}
void* wndmgr_find_close(_obj* wnd, int x, int y)
{
	struct style* sty = 0;
	struct relation* rel = wnd->oreln;

	//find max_w
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
			sty = (void*)(rel->srcfoot);
			if(sty){
				//say("%d,%d, %f,%f,%f,%f\n",x,y,sty->fs.vc[0],sty->fs.vc[1],sty->fs.vr[0],sty->fs.vf[1]);
				if(	(x > sty->fs.vc[0] - sty->fs.vr[0]) &&
					(x < sty->fs.vc[0] - sty->fs.vr[0] + 16) &&
					(y > sty->fs.vc[1] - sty->fs.vf[1] - 16) &&
					(y < sty->fs.vc[1] - sty->fs.vf[1]) )
				{
					return rel;
				}
			}
		}

		rel = samesrcprevdst(rel);
	}
	return rel;
}




int wndmgr_rgba_take(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//pixel_clearcolor(wnd);
	int x = wnd->whdf.ix0;
	int y = wnd->whdf.iy0;

	struct relation* the = wndmgr_find_maxw(wnd);
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

			struct style* sty = rel->psrcfoot;
			int x0 = sty->fs.vc[0] - sty->fs.vr[0];	//(left, top)
			int y0 = sty->fs.vc[1] - sty->fs.vf[1];
			int xn = sty->fs.vc[0] + sty->fs.vr[0]-1;	//(right, bot)
			int yn = sty->fs.vc[1] + sty->fs.vf[1]-1;
			int tx0 =        x0;	//title.(left, top)
			int ty0 =     y0-16;
			int txn = (x0+xn)/2;	//(right, bot)
			int tyn =      y0-1;

			//border
			if(
				wndmgr_mouseat_border(x,y, x0,y0, xn,yn) |
				wndmgr_mouseat_title(x,y, tx0,ty0, txn,tyn)
			){
				drawline_choose((void*)wnd,0xffff00, x0,y0,xn,yn);
			}

			//title.rect
			if(the == rel){
				drawopaque_rect((void*)wnd, 0xc0ff0000, tx0,ty0, txn,tyn);

				drawsolid_rect((void*)wnd, 0xffff00, tx0+0,ty0+0, tx0+15+0,tyn+0);
				drawsolid_rect((void*)wnd, 0x00ffff, tx0+4,ty0+4, tx0+15-4,tyn-4);
			}
			else{
				drawopaque_rect((void*)wnd, 0xc07f007f, tx0,ty0, txn,tyn);

				drawsolid_rect((void*)wnd, 0x7f7f00, tx0+0,ty0+0, tx0+15+0,tyn+0);
				drawsolid_rect((void*)wnd, 0x007f7f, tx0+4,ty0+4, tx0+15-4,tyn-4);
			}

			//title.name
			_obj* ent = rel->pdstchip;
			drawstring_fit((void*)wnd, 0xffffff, tx0,ty0, txn,tyn, (void*)&ent->hfmt, 8);
		}
next:
		rel = samesrcnextdst(rel);
	}

	//float
	int qx = wnd->whdf.width-64;
	int qy = wnd->whdf.height-64;
	u32 c = timeread();
	drawsolid_rect((void*)wnd, c, qx-32, qy-32, qx+32, qy+32);
	drawsolid_circle((void*)wnd, ~c, qx, qy, 32);

	//mouse
	drawline((void*)wnd, 0xffff00, x-16, y, x+16, y);
	drawline((void*)wnd, 0xffff00, x, y-16, x, y+16);
	return 0;
}
int wndmgr_rgba_give(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@rgbanode_write:%p,%x\n", wnd,foot);
	//printmemory(buf,16);

	struct relation* hit = 0;
	struct relation* the = wndmgr_find_maxw(wnd);

	struct event* ev = buf;
	if('p' == (ev->what&0xff)){
		short* pp = (short*)ev;
		wnd->whdf.ix0 += pp[0];
		if(wnd->whdf.ix0 < 0)wnd->whdf.ix0 = 0;
		if(wnd->whdf.ix0 >= wnd->whdf.width)wnd->whdf.ix0 = wnd->whdf.width-1;

		wnd->whdf.iy0 += pp[1];
		if(wnd->whdf.iy0 < 0)wnd->whdf.iy0 = 0;
		if(wnd->whdf.iy0 >= wnd->whdf.height)wnd->whdf.iy0 = wnd->whdf.height-1;
	}
	if(0x2b70 == ev->what){
		hit = wndmgr_find_close(wnd, wnd->whdf.ix0, wnd->whdf.iy0);
		//say("the=%p,hit=%p\n",the,hit);
		if(hit){
			struct style* tmp = (void*)(hit->srcfoot);
			tmp->fs.vr[0] /= 2;
			tmp->fs.vr[1] /= 2;
			tmp->fs.vf[0] /= 2;
			tmp->fs.vf[1] /= 2;
		}
		return 0;
	}
	if(0x2d70 == ev->what){
		say("mouse up\n");
		return 0;
	}
	if(0x4070 == ev->what){
		hit = wndmgr_find_hit(wnd, wnd->whdf.ix0, wnd->whdf.iy0);
		//say("the=%p,hit=%p\n",the,hit);
		if(hit && the && (hit != the)){
			struct style* hitsty = (void*)(hit->srcfoot);
			struct style* thesty = (void*)(the->srcfoot);
			hitsty->fs.vc[3] = thesty->fs.vc[3]+1.0;
			the = hit;
		}
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

	int x = wnd->whdf.ix0;
	int y = wnd->whdf.iy0;
	drawline((void*)wnd, 0xffff00, x-16, y, x+16, y);
	drawline((void*)wnd, 0xffff00, x, y-16, x, y+16);
	return 0;
}


int wndmgr_gl41cmdq_take(_obj* mgr,void* foot, _obj* wnd,void* sty)
{
	say("wndmgr_gl41cmdq_take\n");
	gl41cmdq_clear(wnd);
	return 0;
}


int wndmgr_take(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		wndmgr_rgba_take(wnd,foot, stack,sp, arg,key, buf,len);
		break;
	case _gl41list_:
		break;
	case _gl41cmdq_:
		wndmgr_gl41cmdq_take(wnd,foot, caller,area);
		break;
	}
	return 0;
}
int wndmgr_give(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return wndmgr_rgba_give(wnd,foot, stack,sp, arg,key, buf,len);
}





void wndmgr_delete(_obj* act, u8* buf)
{
}
void wndmgr_create(_obj* act, u8* buf)
{
}
