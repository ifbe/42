#include "libuser.h"
#define _mvp_ hex32('m','v','p',0)




void* locate_camera_in_world()
{
	return 0;
}




int gl41wnd0_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int idx, void* buf, int len)
{
	say("@gl41wnd0_read\n");
/*	struct actor* wnd;
	struct relation* rel;

	wnd = self->pchip;
	if(0 == wnd)return 0;

	rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		//read data
		stack[idx+0] = (void*)(rel->src);
		stack[idx+1] = (void*)(rel->dst);
		actorread(stack[idx+1], stack[idx+0], stack, idx+2, 0, 0);

		//render this
		arenawrite(stack[0], stack[1], stack, idx+2, 0, 0);

		rel = samesrcnextdst(rel);
	}
*/
	return 0;
}
int gl41wnd0_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	float x,y,x0,y0,xn,yn;
	short* v;
	struct actor* wnd;
	struct fstyle* sty;
	struct relation* rel;
	struct event* ev;

	ev = buf;
	say("@gl41wnd0_write:%llx,%llx,%llx,%llx\n", ev->why, ev->what, ev->where, ev->when);

	wnd = self->pchip;
	if(0 == wnd)return 0;
	rel = wnd->oreln;
	if(0 == rel)return 0;

	v = (short*)ev;
	if('p' == (ev->what&0xff)){
		while(1){
			if(0 == rel)return 0;
			sty = rel->psrcfoot;
			x0 = sty->vc[0] * wnd->width;
			y0 = sty->vc[1] * wnd->height;
			xn = sty->vq[0] * wnd->width + x0;
			yn = sty->vq[1] * wnd->height + y0;
			x = v[0];
			y = wnd->height-v[1];
			if( (x>x0) && (x<xn) && (y>y0) && (y<yn) )goto found;
			rel = samesrcprevdst(rel);
		}
		return 0;
	}

found:
	self = (void*)(rel->dst);
	peer = (void*)(rel->src);
	actorwrite(self, peer, 0, 0, buf, len);
	return 0;
}
int gl41wnd0_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41wnd0_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41wnd0_delete(struct actor* act)
{
	return 0;
}
int gl41wnd0_create(struct actor* act, void* data)
{
	return 0;
}
