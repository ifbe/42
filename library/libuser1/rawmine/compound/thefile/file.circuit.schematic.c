#include "libuser.h"
static vec3 aaa[6] = {
{-400, 500, 0}, {-500, 500, 0},
{-500, 500, 0}, {-500,-500, 0},
{-500,-500, 0}, {-400,-500, 0}
};
static vec3 bbb[4] = {
{0, -100, 0}, {0, 100, 0},
{0, 0, 0}, {500, 0, 0}
};




static void sch_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j,k;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41solid_rect(wnd, 0x404040, vc, vr, vf);

	u32 acolor = act->ix0 ? 0xff0000 : 0x0000ff;
	u32 bcolor = act->iy0 ? 0xff0000 : 0x0000ff;
	for(k=0;k<3;k++)gl41line(wnd, acolor, aaa[k*2+0], aaa[k*2+1]);
	for(k=0;k<2;k++)gl41line(wnd, bcolor, bbb[k*2+0], bbb[k*2+1]);
}
void sch_read_board(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> sch
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	act = self->pchip;slot = self->pfoot;
	win = peer->pchip;geom = peer->pfoot;
	wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
	wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
	sch_draw_gl41(act,slot, win,geom, wnd,area);
}
int sch_read_child(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* scene;
	struct relation* rel;
	//say("@sch_read\n");

	scene = self->pchip;
	if(0 == scene)return 0;
	rel = scene->orel0;
	if(0 == rel)return 0;

	while(1){
		if(0 == rel)break;
		if(rel->srcfoot)entityread((void*)(rel->dst), (void*)(rel->src), stack, rsp, buf, len);
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int sch_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	if(stack && ('v' == len)){
		sch_read_child(self,peer, stack,rsp, buf,len);
		sch_read_board(self,peer, stack,rsp, buf,len);
	}
	return 0;
}
int sch_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
		case _ev_:{
			if('1' == buf[0])ent->ix0 = 1;
			if('0' == buf[0])ent->ix0 = 0;
			relationwrite(ent, 'a', 0, 0, buf, 0);
			break;
		}
		case 'b':{
			ent->iy0 = buf[0]-0x30;
			break;
		}
	}
	return 0;
}
int sch_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int sch_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@sch_linkup: %.4s\n", &self->flag);
	return 0;
}




int sch_search(struct entity* scene)
{
	return 0;
}
int sch_modify(struct entity* scene)
{
	return 0;
}
int sch_delete(struct entity* scene)
{
	return 0;
}
int sch_create(struct entity* scene, void* str)
{
	say("@sch_create\n");
	scene->ix0 = 0;
	scene->iy0 = 0;
	return 0;
}
