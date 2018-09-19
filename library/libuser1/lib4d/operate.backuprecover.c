#include "libuser.h"
int startfile(void*, int);
int readfile(u64 file, u64 off, u8* mem, u64 len);
int writefile(u64 file, u64 off, u8* mem, u64 len);
int stopfile(int);
//
int openreadclose(void*, int, void*, int);
void printhtmlbody(u8* buf, int len);




struct arena* arena;
void backuper_create(void* addr)
{
	arena = (void*)addr;
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
void scene_backup(u8* buf, int len)
{/*
	if(0 == len)
	{
		while(1)
		{
			if(buf[len] < 0x20)break;
			len++;
		}
	}
	say("%.*s\n", len, buf);
*/
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
void scene_recover(u8* buf, int len)
{
	int j,k,ret;
	struct arena* win;
	u8* mem;
	u8 tmp[256];

	ret = 0;
	while(1)
	{
		if(buf[ret] < 0x20)break;
		tmp[ret] = buf[ret];
		ret++;
	}
	tmp[ret] = 0;
	say("name: %.*s\n", ret, tmp);

	win = arenacreate(_html_, 0);
	if(0 == win)return;
	say("win: %llx\n", win);

	mem = memorycreate(0x100000);
	if(0 == mem)return;
	say("mem: %llx\n", mem);

	win->buf = mem;
	ret = openreadclose(tmp, 0, mem, 0x100000);
	if(0 >= ret)return;
	say("read:%d\n", ret);

	printhtmlbody(mem, ret);
}
