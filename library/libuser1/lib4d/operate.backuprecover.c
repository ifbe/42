#include "libuser.h"
int startfile(void*, int);
int readfile(u64 file, u64 off, u8* mem, u64 len);
int writefile(u64 file, u64 off, u8* mem, u64 len);
int stopfile(int);
//
int openreadclose(void*, int, void*, int);
void printhtmlbody(u8* buf, int len);




struct arena* arena;
struct actor* actor;
void backuper_create(void* addr)
{
	arena = (void*)addr;
	actor = (void*)(addr+0x100000);
}




void scene_backup_pix(struct arena* win)
{
	int x,y;
	int w,h,s;
	int fd,ret;
	u8 head[0x200];
	u8* buf;

	fd = startfile("fuck.ppm", 'w');
	if(fd <= 0)return;

	w = win->width;
	h = win->height;
	s = win->stride;
	buf = win->buf;

	ret = mysnprintf(head, 0x200, "P6\n%d,%d\n255\n", w, h);
	writefile(fd, 0, head, ret);

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)writefile(fd, ret + (y*w+x)*3, buf + (y*s+x)*4, 3);
	}

	stopfile(fd);
}
void scene_backup_vbo(struct arena* win)
{
	int j;
	struct datapair* mod = win->mod;
	struct glsrc* src;
	for(j=0;j<32;j++)
	{
		src = &mod[j].src;
		if(0 == src->vbuf)continue;
		say(
			"%d{\n"
			"	vs:%llx,fs:%llx\n"
			"	tex0=%llx,tfmt=%x,tw=%x,th=%x\n"
			"	vbuf=%llx,vfmt=%x,vw=%x,vh=%x\n"
			"	ibuf=%llx,ifmt=%x,iw=%x,ih=%x\n"
			"}\n",
			j,
			src->vs, src->fs,
			src->tex[0], src->tex_fmt[0], src->tex_w[0], src->tex_h[0],
			src->vbuf, src->vbuf_fmt, src->vbuf_w, src->vbuf_h,
			src->ibuf, src->ibuf_fmt, src->ibuf_w, src->ibuf_h
		);
	}
}
void scene_backup_world()
{
	int j;
	struct arena* win;
	struct actor* act;
	say("backup world!\n");

	//arena
	say("<arena>\n");
	for(j=0;j<16;j++)
	{
		win = &arena[j];
		if(0 == win->type)break;
		say("<win type=%.8s fmt=%.8s></win>\n", &win->type, &win->fmt);
	}
	say("</arena>\n");

	//actor
	say("<actor>\n");
	for(j=0;j<64;j++)
	{
		act = &actor[j];
		if(0 == act->type)break;
		say("<act type=%.8s name=%.8s></act>\n", &act->type, &act->name);
	}
	say("</actor>\n");
}
void scene_backup(u8* buf, int len)
{
	if('=' != buf[0])
	{
		scene_backup_world();
		return;
	}
	buf++;

	int j;
	struct arena* win;
	for(j=0;j<16;j++)
	{
		win = &arena[j];
		if(_win_ != win->type)continue;

		if(_vbo_ == win->fmt)
		{
			scene_backup_vbo(win);
			break;
		}
		if('g' == ((win->fmt&0xff00)>>8))
		{
			scene_backup_pix(win);
			break;
		}
	}
}




void scene_recover_html(struct str* name)
{
	int j;
	void* buf;
	struct arena* win;
	struct str* data;

	//register arena
	win = arenacreate(_html_, 0);
	if(0 == win)return;
	say("win: %llx\n", win);

	//alloc memory
	buf = memorycreate(0x100000);
	if(0 == buf)return;
	say("buf: %llx\n", buf);
	win->buf = buf;

	//read file
	data = win->buf;
	j = openreadclose(name->buf, 0, data->buf, 0xf0000);
	if(j <= 0)return;
	say("read:%d\n", j);
	data->len = j;

	printhtmlbody(data->buf, data->len);
}
void scene_recover_world()
{
	say("recover world!\n");
}
void scene_recover(u8* buf, int len)
{
	int j;
	u8 tmp[256];
	struct str* name;

	if('=' != buf[0])
	{
		scene_recover_world();
		return;
	}
	buf++;

	//copy into local buf, because '\n'
	j = 0;
	name = (void*)tmp;
	while(1)
	{
		if(buf[j] < 0x20)break;
		name->buf[j] = buf[j];
		j++;
	}
	name->buf[j] = 0;
	name->len = j;
	say("name: %.*s\n", name->len, name->buf);

	scene_recover_html(name);
}
