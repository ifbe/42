#include "libuser.h"




void* locate_camera_in_world()
{
	return 0;
}




int gl41draw_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{/*
	struct relation* rel = self->orel0;

	while(1){
		if(0 == rel)break;

		//
		gl41draw_prerequ();
		gl41draw_thiscam();

		rel = samesrcnextdst();
	}
*/
	return 0;
}
int gl41draw_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{/*
	struct relation* rel;
	struct style* sty;

	rel = self->oreln;
	while(1){
		if(0 == rel)break;

		sty = (void*)(rel->srcfoot);
		if(	(x > sty->vc[0]) &&
			(x < sty->vq[0]) &&
			(y > sty->vc[1]) &&
			(y < sty->vq[1]) )goto found;

		rel = samesrcprevdst();
	}
	return 0;

found:
	//struct actor* cam = (void*)(rel->dstchip);
	world = locate_camera_in_world();
	actorwrite(self, peer, ev);
*/
	return 0;
}
int gl41draw_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41draw_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41draw_delete(struct actor* act)
{
	return 0;
}
int gl41draw_create(struct actor* act)
{
	return 0;
}
