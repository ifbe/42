#include "libuser.h"
#define PATHBUF buf0
#define LISTBUF buf1
int copypath(void* path, void* data);
void readfolder(void* url, int fd, void* arg, int off, void* buf, int len);
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_whcam(struct entity* wnd, struct style* area);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




static int fslist_find(u8* name, int id, int* head, int* tail)
{
	int j,k = 0;
	for(j=0;j<0x1000;j++){
		if('\n' == name[j]){
			if(id <= 0){
				*head = k;
				*tail = j;
				return 1;
			}
			k = j+1;
			id--;
		}
	}
	return 0;
}
static int fslist_copy(u8* path, u8* name)
{
	int j,k;
	for(j=0;j<0x200;j++){
		if(path[j] < 0x20)goto found;
	}
	return 0;

found:
	k = j;
	path[j] = '/';
	j += copypath(path+j+1, name) + 1;
	path[j] = 0;
	say("%s\n", path);
	return k;
}
static void fslist_write_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area, struct event* ev)
{
	if(0x2b70 == ev->what){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
		say("%f,%f\n",xyz[0],xyz[1]);

		float h = area->fs.vq[1] * wnd->fbheight;
		int id = (1.0-xyz[1])*h/32;
		say("%d\n",id);

		int head,tail,j,k;
		u8* path = ent->PATHBUF;
		u8* name = ent->LISTBUF;
		fslist_find(name, id, &head, &tail);
		if(tail-head>6){
		if(0 == ncmp(name+tail-5, ".myml", 5)){
			k = fslist_copy(path, name+head);
			eventwrite((u64)path, _myml_, 0, 0);
			return;
		}
		}

		fslist_copy(path, name+head);
		for(j=0;j<0x1000;j++)name[j] = 0;
		readfolder(path,0, 0,0, name,0x10000);
	}
}




static void fslist_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fslist_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fslist_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fslist_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fslist_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void fslist_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tc;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41solid_rect(wnd,0x000080, vc,vr,vf);

	for(j=0;j<3;j++)tc[j] = vc[j] + vt[j]/100.0;
	carvetext(wnd, 0xff00ff, tc,vr,vf, act->LISTBUF, 0x10000);
}
static void fslist_read_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area)
{
	int j;
	struct fstyle fs;
	for(j=0;j<3;j++)fs.vc[j] = fs.vr[j] = fs.vf[j] = fs.vt[j] = 0.0;
	fs.vr[0] = area->fs.vq[0] * wnd->fbwidth / 2.0;
	fs.vf[1] = area->fs.vq[1] * wnd->fbheight/ 2.0;
	fs.vt[2] = 1.0;

	gl41data_before(wnd);
	fslist_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_whcam(wnd, area);
	gl41data_after(wnd);
}




static void fslist_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != key)break;
		fslist_read_bywnd(ent,slot, wnd,area);break;
	}
	}
}
static void fslist_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:fslist_write_bywnd(ent,slot, wnd,area, buf);break;
	}
}
static void fslist_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void fslist_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void fslist_search(struct entity* act)
{
}
static void fslist_modify(struct entity* act)
{
}
static void fslist_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->LISTBUF){
		memorydelete(act->LISTBUF);
		act->LISTBUF = 0;
	}
}
static void fslist_create(struct entity* act, void* arg, int argc, u8** argv)
{
	if(0 == act)return;

	act->PATHBUF = memorycreate(0x10000, 0);
	if(0 == arg)arg = "datafile/myml";
	copypath(act->PATHBUF, arg);

	act->LISTBUF = memorycreate(0x10000, 0);
	readfolder(act->PATHBUF,0, 0,0, act->LISTBUF,0x10000);
}




void fslist_register(struct entity* p)
{
	p->fmt = hex64('f','s','l','i','s','t', 0, 0);

	p->oncreate = (void*)fslist_create;
	p->ondelete = (void*)fslist_delete;
	p->onsearch = (void*)fslist_search;
	p->onmodify = (void*)fslist_modify;

	p->onlinkup = (void*)fslist_linkup;
	p->ondiscon = (void*)fslist_discon;
	p->onread  = (void*)fslist_read;
	p->onwrite = (void*)fslist_write;
}
