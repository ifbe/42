#include "libuser.h"




int eeworld_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct actor* world;
	struct actor* kirch;

	world = (void*)self->chip;
	if(0 == world)return 0;

	kirch = (void*)peer->chip;
	if(0 == kirch)return 0;

	say("@eeworld_read:%.8s, %.8s\n", &world->type, &kirch->type);
/*
	rel = world->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			actorread(self, peer, kirch, 0);
		}
next:
		rel = samesrcnextdst(rel);
	}
*/
	relationread(world, _src_, 0, 0, 0, 0);
	return 0;
}
int eeworld_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	return 0;
}
int eeworld_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int eeworld_start(struct halfrel* self, struct halfrel* peer)
{
	say("@eeworld_start\n");
	return 0;
}




int eeworld_search(struct actor* world)
{
	return 0;
}
int eeworld_modify(struct actor* world)
{
	return 0;
}
int eeworld_delete(struct actor* world)
{
	return 0;
}
int eeworld_create(struct actor* world, void* str)
{
	say("@eeworld_create\n");
	return 0;
}
