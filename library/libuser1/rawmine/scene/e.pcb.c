#include "libuser.h"




static void pcb_draw_vbo(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int x,y,j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	carvesolid_rect(ctx, 0x404000, vc, vr, vf);
}
void pcb_read_board(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> pcb
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	act = self->pchip;slot = self->pfoot;
	win = peer->pchip;geom = peer->pfoot;
	wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
	wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
	pcb_draw_vbo(act,slot, win,geom, wnd,area);
}
int pcb_read_child(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* scene;
	struct relation* rel;
	//say("@pcb_read\n");

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
int pcb_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	if(stack && ('v' == len)){
		pcb_read_child(self,peer, stack,rsp, buf,len);
		pcb_read_board(self,peer, stack,rsp, buf,len);
	}
	return 0;
}
int pcb_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return 0;
}
int pcb_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int pcb_start(struct halfrel* self, struct halfrel* peer)
{
	say("@pcb_start\n");
	return 0;
}




int pcb_search(struct entity* scene)
{
	return 0;
}
int pcb_modify(struct entity* scene)
{
	return 0;
}
int pcb_delete(struct entity* scene)
{
	return 0;
}
int pcb_create(struct entity* scene, void* str)
{
	say("@pcb_create\n");
	return 0;
}
