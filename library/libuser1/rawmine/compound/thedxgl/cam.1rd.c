#include "libuser.h"
void fixmatrix_transpose(float* m, struct fstyle* sty);




static int firstperson_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int j;
	struct relation* rel;
	struct entity* tar;
	u8* src;
	u8* dst;

	//3rdcam's target
	rel = act->irel0;
	while(1)
	{
		if(0 == rel)return 0;

		if(_ent_ == rel->srctype)
		{
			tar = (void*)(rel->srcchip);
			if(0 == tar)return 0;
			else goto found;
		}

		rel = samedstnextsrc(rel);
	}
	return 0;

found:
	//src = (void*)(&tar->camera);
	//dst = (void*)(   &win->camera);
	//for(j=0;j<sizeof(struct style);j++)dst[j] = src[j];
	//carvefrustum(win, &win->camera);
	return 0;
}




void firstperson_frustum(struct fstyle* d, struct fstyle* s)
{
	float x,y,z,n;
	d->vc[0] = s->vc[0];
	d->vc[1] = s->vc[1];
	d->vc[2] = s->vc[2];


	x = s->vr[0];
	y = s->vr[1];
	z = s->vr[2];
	n = squareroot(x*x + y*y + z*z);
	d->vr[0] = x / n;
	d->vr[1] = y / n;
	d->vr[2] = z / n;
	//d->vr[3] = 0.7;
	d->vl[0] = -x / n;
	d->vl[1] = -y / n;
	d->vl[2] = -z / n;
	//d->vl[3] = -0.7;


	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	n = squareroot(x*x + y*y + z*z);
	d->vt[0] = x / n;
	d->vt[1] = y / n;
	d->vt[2] = z / n;
	//d->vt[3] = 0.7;
	d->vb[0] = -x / n;
	d->vb[1] = -y / n;
	d->vb[2] = -z / n;
	//d->vb[3] = -0.7;


	x = s->vf[0];
	y = s->vf[1];
	z = s->vf[2];
	n = squareroot(x*x + y*y + z*z);
	d->vn[0] = x / n;
	d->vn[1] = y / n;
	d->vn[2] = z / n;
	//d->vn[3] = 1.0;
	d->vf[0] = x / n;
	d->vf[1] = y / n;
	d->vf[2] = z / n;
	//d->vf[3] = 1e20;
}
static void firstperson_matrix(
	struct entity* act, struct style* frustum,
	struct entity* win, struct style* wingeom,
	u8* buf, int len)
{/*
	struct relation* rel;
	struct entity* r;
	struct fstyle* s;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		if(hex32('g','e','o','m') == rel->dstflag){
			s = (void*)(rel->srcfoot);
			r = (void*)(rel->srcchip);
			break;
		}
		rel = samedstnextsrc(rel);
	}
	if(0 == s)return;


	float* m = act->buf;
	firstperson_frustum(&frustum->f, s);
	fixmatrix_transpose(m, &frustum->f);
	//printmat4(m);


	u64* p = (void*)buf;
	struct glsrc* src = (void*)(buf+0x20);

	p[0] = (u64)src;
	p[1] = (u64)r->gl_light;
	p[2] = (u64)r->gl_solid;
	p[3] = (u64)r->gl_opaque;


	src->arg_fmt[0] = 'm';
	src->arg_name[0] = "cammvp";
	src->arg_data[0] = m;

	src->arg_fmt[1] = 'v';
	src->arg_name[1] = "camxyz";
	src->arg_data[1] = act->camera.vc;
*/
}



static void firstperson_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
/*	switch(self->flag){
		case _cam_:firstperson_matrix(act, pin, win, sty, buf, len);break;
	}*/
}
static void firstperson_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void firstperson_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void firstperson_start(struct halfrel* self, struct halfrel* peer)
{
    say("@firstperson_start\n");
}




static void firstperson_search(struct entity* act)
{
}
static void firstperson_modify(struct entity* act)
{
}
static void firstperson_delete(struct entity* act)
{
}
static void firstperson_create(struct entity* act, void* addr)
{
    say("@firstperson_create\n");
}




void firstperson_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('1', 'r', 'd', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)firstperson_create;
	p->ondelete = (void*)firstperson_delete;
	p->onsearch = (void*)firstperson_search;
	p->onmodify = (void*)firstperson_modify;

	p->onstart = (void*)firstperson_start;
	p->onstop  = (void*)firstperson_stop;
	p->onread  = (void*)firstperson_read;
	p->onwrite = (void*)firstperson_write;
}
