#include "libuser.h"
#define PATHBUF buf0
#define LISTBUF buf1
int copypath(void* path, void* data);
int readfolder(void* url, int fd, void* arg, int off, void* buf, int len);
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_whcam(struct entity* wnd, struct style* area);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);


static u32 rainbow[8] = {
0xff0000, 0xff8000, 0xffff00, 0x00ff00,
0x00ffff, 0x0000ff, 0x8000ff, 0x404040};


static int fslist_find(struct str* list, int id, u8** head, u8** tail)
{
	int j,k = 0;
	for(j=0;j<0x1000;j++){
		if('\n' == list->buf[j]){
			if(id <= 0){
				*head = list->buf+k;
				*tail = list->buf+j;
				return 1;
			}
			k = j+1;
			id--;
		}
	}
	return 0;
}
static int fslist_copy(struct str* path, u8* name)
{
	int j;
	if('.' == name[0]){
		if(0x20 > name[1])return 0;
		if('.' == name[1]){
			for(j=path->len-1;j>0;j--){
				if('/' == path->buf[j]){
					if(j == path->len-1)continue;
					path->buf[j+1] = 0;
					path->len = j+1;
					break;
				}
			}
			say("%s\n", path->buf);
			return 0;
		}
	}

	path->len += copypath(path->buf+path->len, name);
	path->buf[path->len] = 0;
	say("%s\n", path->buf);
	return 0;
}
static void fslist_write_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area, struct event* ev)
{
	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
		say("%f,%f\n",xyz[0],xyz[1]);

		float dx = area->fs.vq[0] * wnd->width;
		float dy = area->fs.vq[1] * wnd->height;
		ent->ix0 = (int)(8*xyz[0]);
		ent->iy0 = (int)(8*(1.0-xyz[1])*dy/dx);
		say("%d,%d\n",ent->ix0,ent->iy0);

		if(0x2d70 == ev->what){
			int id = ent->ix0 + (ent->iy0*8);
			say("%d\n",id);

			int j,k;
			struct str* path = ent->PATHBUF;
			struct str* list = ent->LISTBUF;

			u8* head = 0;
			u8* tail = 0;
			fslist_find(list, id, &head, &tail);
			if(0 == head)return;	//notfound

			fslist_copy(path, head);
			if(tail-head > 6){
				if(0 == ncmp(tail-5, ".myml", 5)){
					eventwrite((u64)path->buf, _myml_, 0, 0);
					return;
				}
				//if(0 == ncmp(list+tail-5, ".mython", 7)){
				//}
			}

			for(j=0;j<0x10000;j++)list->buf[j] = 0;
			list->len = readfolder(path->buf,0, 0,0, list->buf,0x10000);
			//say("%s\n",list->buf);
		}
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
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(wnd,0x000080, vc,vr,vf);

	struct str* path = act->PATHBUF;
	if(0 == path)return;
	struct str* list = act->LISTBUF;
	if(0 == list)return;

	int x,y,j;
	vec3 kr,kf;
	for(j=0;j<3;j++){kr[j] = vr[j]/8;kf[j] = vf[j];}
	vec3_setlen(kf, vec3_getlen(kr));

	float scale;
	int bg,fg,head,tail,cnt;
	vec3 tc,tr,tf;
	cnt = 0;
	head = 0;
	for(tail=0;tail<list->len;tail++){
		if('\n' > list->buf[tail])break;
		if('\n'== list->buf[tail]){
			y = cnt/8;
			x = cnt%8;
			if((x==act->ix0)&&(y==act->iy0)){
				bg = 0xffffff;
				fg = 0xff0000;
				scale = 1.1;
			}
			else{
				bg = rainbow[x];
				fg = 0xffffff;
				scale = 0.9;
			}

			for(j=0;j<3;j++){
				tc[j] = vc[j] -vr[j] +vf[j];
				tc[j]+= kr[j]*(2*x+1) -kf[j]*(2*y+1);
				tr[j] = kr[j]*scale;
				tf[j] = kf[j]*scale;
			}
			gl41solid_rect(wnd, bg, tc,tr,tf);

			for(j=0;j<3;j++){
				tc[j] += vt[j]/100.0;
				tr[j] = tr[j];
				tf[j] = tf[j]/4.0;
			}
			carvestring_center(wnd,fg, tc,tr,tf, list->buf+head,tail-head);

			cnt += 1;
			head = tail+1;
		}
	}

	for(j=0;j<3;j++){tr[j] = vr[j];tf[j] = vf[j];}
	vec3_setlen(tr, 32);
	vec3_setlen(tf, 32);
	for(j=0;j<3;j++){tc[j] = vc[j] -vr[j] -vf[j] +vt[j]/100.0;}
	carvestring(wnd,0xffffff, tc,tr,tf, path->buf,path->len);
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




static void fslist_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _gl41full_:{
		if('v' != key)break;
		fslist_read_bywnd(ent,slot, wnd,area);break;
	}
	}
}
static void fslist_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _gl41full_:fslist_write_bywnd(ent,slot, wnd,area, buf);break;
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
	if(act->PATHBUF){
		memorydelete(act->PATHBUF);
		act->PATHBUF = 0;
	}
	if(act->LISTBUF){
		memorydelete(act->LISTBUF);
		act->LISTBUF = 0;
	}
}
static void fslist_create(struct entity* act, void* arg, int argc, u8** argv)
{
	if(0 == arg)arg = "datafile/myml/";

	struct str* path = act->PATHBUF = memorycreate(0x10000, 0);
	path->len = copypath(path->buf, arg);

	struct str* list = act->LISTBUF = memorycreate(0x10000, 0);
	list->len = readfolder(path->buf,0, 0,0, list->buf,0x10000);
//say("%d,%s\n", list->len, list->buf);
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
	p->ontaking = (void*)fslist_taking;
	p->ongiving = (void*)fslist_giving;
}
