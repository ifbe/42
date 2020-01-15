#include "libuser.h"




//-4: wnd, area
//-3: cam, 0
//-2: cam, 0
//-1: world, geom of cam
int scene3d_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* scene;
	struct relation* rel;
	//say("@scene3d_read\n");

	scene = self->pchip;
	if(0 == scene)return 0;
	rel = scene->orel0;
	if(0 == rel)return 0;

	while(1){
		if(0 == rel)break;
		entityread((void*)(rel->dst), (void*)(rel->src), stack, rsp, buf, len);
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int scene3d_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return 0;
}
int scene3d_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int scene3d_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@scene3d_linkup\n");
	return 0;
}




int scene3d_search(struct entity* scene)
{
	return 0;
}
int scene3d_modify(struct entity* scene)
{
	return 0;
}
int scene3d_delete(struct entity* scene)
{
	return 0;
}
int scene3d_create(struct entity* scene, void* str)
{
	say("@scene3d_create\n");
	return 0;
}
