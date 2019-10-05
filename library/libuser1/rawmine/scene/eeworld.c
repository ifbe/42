#include "libuser.h"




int eeworld_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct relation* rel;
	struct actor* act = self->pchip;

	rel = act->orel0;
	while(1){
		if(0 == rel)break;
		say("%llx.%.4s@%llx.%.4s\n", rel->srcchip, &rel->srcflag, rel->dstchip, &rel->dstflag);
		rel = samesrcnextdst(rel);
	}
	say("\n");
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
