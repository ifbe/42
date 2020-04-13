#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)




void frame3d_draw_gl41(struct entity* scene, struct entity* wnd)
{
	struct relation* rel;
	struct entity* ent;
	struct fstyle* sty;
	vec3 tc,tr,tf,tt;
	int j;
	//say("@frame3d_read: %.8s\n", &scene->fmt);

	rel = scene->orel0;
	while(1){
		if(0 == rel)break;
		ent = rel->pdstchip;
		sty = rel->psrcfoot;
		if(sty){
			//say("%llx,%llx\n",ent,sty);
			for(j=0;j<3;j++){
				tr[j] = sty->vr[j];
				tf[j] = sty->vf[j];
				tt[j] = sty->vt[j];
				tc[j] = sty->vc[j];
			}
			gl41line_prism4(wnd, 0xffffff, sty->vc, sty->vr, sty->vf, sty->vt);

			for(j=0;j<3;j++)tc[j] += -tr[j] -tf[j] +tt[j];
			vec3_setlen(tr, 32);
			vec3_setlen(tf, 32);
			carvestring(wnd, 0xffffff, tc,tr,tf, (void*)&ent->fmt, 8);
		}
		rel = samesrcnextdst(rel);
	}
}
int frame3d_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	if(stack && ('v' == len)){
		struct entity* ent = self->pchip;
		if(0 == ent)return 0;

		struct halfrel* aa[2];
		int ret = relationsearch(ent, _tar_, &aa[0], &aa[1]);
		if(ret <= 0)return 0;

		struct entity* tar = aa[1]->pchip;
		if(0 == tar)return 0;

		struct entity* wnd = stack[rsp-4]->pchip;
		if(0 == wnd)return 0;

		frame3d_draw_gl41(tar, wnd);
	}
	return 0;
}




int frame3d_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return frame3d_read_bycam(self,peer, stack,rsp, buf,len);
}
int frame3d_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return 0;
}
int frame3d_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int frame3d_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@frame3d_linkup\n");
	return 0;
}




int frame3d_search(struct entity* scene)
{
	return 0;
}
int frame3d_modify(struct entity* scene)
{
	return 0;
}
int frame3d_delete(struct entity* scene)
{
	return 0;
}
int frame3d_create(struct entity* scene, void* str)
{
	say("@frame3d_create\n");
	return 0;
}
