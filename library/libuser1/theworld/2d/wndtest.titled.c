#include "libuser.h"




int wndmgr_mouseat_title(int x,int y, int x0,int y0, int xn,int yn)
{
	if(x < x0)return 0;
	if(x > xn)return 0;
	if(y > y0)return 0;
	if(y < yn)return 0;
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
void* wndmgr_find_maxw(struct supply* wnd)
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
void* wndmgr_find_hit(struct supply* wnd, int x, int y)
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




int wndmgr_take(_sup* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//pixel_clearcolor(wnd);
	int x = wnd->ix0;
	int y = wnd->iy0;

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
			int xn = sty->fs.vc[0] + sty->fs.vr[0];	//(right, bot)
			int yn = sty->fs.vc[1] + sty->fs.vf[1];
			int xq = sty->fs.vc[0];		//(mid, above top 16 pixel)
			int yq = y0-16;
			int xt = x0+16;				//(left+16, top-16)
			int yt = y0-16;

			//border
			if(
				wndmgr_mouseat_border(x,y, x0,y0, xn,yn) |
				wndmgr_mouseat_title(x,y, x0,y0, xq,yq)
			){
				drawline_rect((void*)wnd,0xffff00, x0,y0,xn,yn);
			}

			//title.rect
			if(the == rel){
				drawopaque_rect((void*)wnd, 0xc0ff0000, x0,y0, xq,yq);

				drawsolid_rect((void*)wnd, 0xffff00, x0,y0, xt,yt);
				drawsolid_rect((void*)wnd, 0x00ffff, x0+4,y0-4, xt-4,yt+4);
			}
			else{
				drawopaque_rect((void*)wnd, 0xc07f007f, x0,y0, xq,yq);

				drawsolid_rect((void*)wnd, 0x7f7f00, x0,y0, xt,yt);
				drawsolid_rect((void*)wnd, 0x007f7f, x0+4,y0-4, xt-4,yt+4);
			}

			//title.name
			struct entity* ent = rel->pdstchip;
			drawstring_fit((void*)wnd, 0xffffff, x0,y0, xq,yq, (void*)&ent->fmt, 8);
		}
next:
		rel = samesrcnextdst(rel);
	}

	//float
	int qx = wnd->width-64;
	int qy = wnd->height-64;
	u32 c = timeread();
	drawsolid_rect((void*)wnd, c, qx-32, qy-32, qx+32, qy+32);
	drawsolid_circle((void*)wnd, ~c, qx, qy, 32);

	//mouse
	drawline((void*)wnd, 0xffff00, x-16, y, x+16, y);
	drawline((void*)wnd, 0xffff00, x, y-16, x, y+16);
	return 0;
}
int wndmgr_give(_sup* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@rgbanode_write:%p,%x\n", wnd,foot);
	//printmemory(buf,16);

	struct relation* hit = 0;
	struct relation* the = wndmgr_find_maxw(wnd);

	struct event* ev = buf;
	if(0x4070 == ev->what){
		short* pp = (short*)ev;
		wnd->ix0 += pp[0];
		if(wnd->ix0 < 0)wnd->ix0 = 0;
		if(wnd->ix0 >= wnd->width)wnd->ix0 = wnd->width-1;

		wnd->iy0 += pp[1];
		if(wnd->iy0 < 0)wnd->iy0 = 0;
		if(wnd->iy0 >= wnd->height)wnd->iy0 = wnd->height-1;

		hit = wndmgr_find_hit(wnd, wnd->ix0, wnd->iy0);
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

	int x = wnd->ix0;
	int y = wnd->iy0;
	drawline((void*)wnd, 0xffff00, x-16, y, x+16, y);
	drawline((void*)wnd, 0xffff00, x, y-16, x, y+16);
	return 0;
}