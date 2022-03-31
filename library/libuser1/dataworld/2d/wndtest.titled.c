#include "libuser.h"
#define LASTDRAWTIME whdf.iw0
#define LASTCOPYTIME whdf.iwn
int gl41cmdq_clear(void*);


struct permgr{
	//normal
	void* mouse;
	void* chosen;

	//special node
	void* ball;
	void* edge;

	//event debug
	void* evdbg;
};


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
	struct relation* tar = rel;

	//find max_w
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dstnodetype){
			sty = (void*)(rel->srcfoot);
			if(sty){
				//say("%f\n", sty->fs.vc[3]);
				if(max < sty->fs.vc[3]){
					tar = rel;
					max = sty->fs.vc[3];
				}
			}
		}

		rel = samesrcprevdst(rel);
	}
	return tar;
}
void* wndmgr_find_hit(_obj* wnd, int x, int y)
{
	struct style* sty = 0;
	struct relation* rel = wnd->oreln;

	//find max_w
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dstnodetype){
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

		if(_ent_ == rel->dstnodetype){
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




int wndmgr_rgba_take(_obj* mgr,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* wnd = mgr;
	if(_rgba_ != mgr->hfmt)wnd = stack[sp-2].pchip;
	if(_rgba_ != wnd->hfmt)return 0;
	//pixel_clearcolor(wnd);

	int mx = mgr->whdf.ix0;
	int my = mgr->whdf.iy0;
	struct relation* tar = wndmgr_find_maxw(mgr);
	struct relation* rel = mgr->orel0;
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
				wndmgr_mouseat_border(mx,my, x0,y0, xn,yn) |
				wndmgr_mouseat_title(mx,my, tx0,ty0, txn,tyn)
			){
				drawline_choose((void*)wnd,0xffff00, x0,y0,xn,yn);
			}

			//title.rect
			if(tar == rel){
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

	//previous event
	struct permgr* pw = (void*)mgr->priv_256b;
	if(pw->evdbg){
		drawsolid_rect((void*)wnd, 0, wnd->whdf.width/4, wnd->whdf.height-32, wnd->whdf.width*3/4, wnd->whdf.height-16);
		drawhexdump(wnd, 0xff00ff, wnd->whdf.width/4, wnd->whdf.height-32, pw->evdbg, 16);
	}

	//button and time
	int qx = wnd->whdf.width-64;
	int qy = wnd->whdf.height-64;
	u64 now = timeread_us();
	drawsolid_rect((void*)wnd, now, qx-32, qy-32, qx+32, qy+32);
	drawsolid_circle((void*)wnd, ~now, qx, qy, 32);
	drawdecimal((void*)wnd, 0xff00ff, qx-32, qy-16, wnd->LASTDRAWTIME);
	drawdecimal((void*)wnd, 0xff00ff, qx-32, qy+00, wnd->LASTCOPYTIME);

	//mouse
	drawline((void*)wnd, 0xffff00, mx-16, my, mx+16, my);
	drawline((void*)wnd, 0xffff00, mx, my-16, mx, my+16);
	return 0;
}
int wndmgr_rgba_give(_obj* mgr,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@rgbanode_write:%p,%x\n", mgr,foot);
	//printmemory(buf,16);

	_obj* wnd = mgr;
	if(_rgba_ != mgr->hfmt)wnd = stack[sp-2].pchip;
	if(_rgba_ != wnd->hfmt)return 0;

	struct event* ev = buf;
	struct permgr* pw = (void*)mgr->priv_256b;
	pw->evdbg = ev;

	struct relation* hit = 0;
	struct relation* tar = wndmgr_find_maxw(mgr);

	if(point_type == (ev->what&point_mask)){
		short* pp = (short*)ev;
		int tx = pp[0];
		int ty = pp[1];

		//mouse position
		switch(ev->what){
		case point_abs:
			mgr->whdf.ix0 = tx;
			mgr->whdf.iy0 = ty;
			break;
		case point_per:
			mgr->whdf.ix0 = (wnd->whdf.width * tx)>>15;
			mgr->whdf.iy0 = (wnd->whdf.height * ty)>>15;
			break;
		case point_dlt:
			mgr->whdf.ix0 += tx;
			if(mgr->whdf.ix0 < 0)mgr->whdf.ix0 = 0;
			if(mgr->whdf.ix0 >= wnd->whdf.width)mgr->whdf.ix0 = wnd->whdf.width-1;

			mgr->whdf.iy0 += ty;
			if(mgr->whdf.iy0 < 0)mgr->whdf.iy0 = 0;
			if(mgr->whdf.iy0 >= mgr->whdf.height)mgr->whdf.iy0 = wnd->whdf.height-1;
			break;

		case 0x2b70:
			say("mouse dn: %d,%d\n",tx,ty);

			//check close
			hit = wndmgr_find_close(mgr, mgr->whdf.ix0, mgr->whdf.iy0);
			//say("tar=%p,hit=%p\n",tar,hit);
			if(hit){
				struct style* tmp = (void*)(hit->srcfoot);
				tmp->fs.vr[0] /= 2;
				tmp->fs.vr[1] /= 2;
				tmp->fs.vf[0] /= 2;
				tmp->fs.vf[1] /= 2;
				return 0;
			}

			//check choose
			hit = wndmgr_find_hit(mgr, mgr->whdf.ix0, mgr->whdf.iy0);
			//say("tar=%p,hit=%p\n",tar,hit);
			if(hit && tar && (hit != tar)){
				struct style* hitsty = (void*)(hit->srcfoot);
				struct style* tarsty = (void*)(tar->srcfoot);
				hitsty->fs.vc[3] = tarsty->fs.vc[3]+1.0;
				return 0;
			}
			break;

		case 0x2d70:
			say("mouse up: %d,%d\n",tx,ty);
			break;
		}
	}//mouse event

//say("max=%f,tar=%p\n",max,tar);
	if(tar){
		stack[sp+0].pchip = tar->psrcchip;
		stack[sp+0].pfoot = tar->psrcfoot;
		stack[sp+0].foottype = tar->srcfoottype;
		stack[sp+1].pchip = tar->pdstchip;
		stack[sp+1].pfoot = tar->pdstfoot;
		stack[sp+1].foottype = tar->dstfoottype;
		entity_giveby(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
	}

	int x = mgr->whdf.ix0;
	int y = mgr->whdf.iy0;
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
	//both window and manager
	if(_rgba_ == wnd->hfmt){
		wndmgr_rgba_take(wnd,foot, stack,sp, arg,key, buf,len);
		return 0;
	}

	//manager called by window
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
int wndmgr_attach(_obj* wnd,void* foot)
{
	return 0;
}
int wndmgr_detach(_obj* wnd,void* foot)
{
	return 0;
}





void wndmgr_reader(_obj* act, u8* xxx, void* arg,int key, void* buf,int len)
{
}
void wndmgr_writer(_obj* act, u8* xxx, void* arg,int key, void* buf,int len)
{
}
void wndmgr_delete(_obj* act, u8* xxx)
{
}
void wndmgr_create(_obj* act, u8* xxx)
{
}
