#include "libuser.h"
int startfile(void*, int);
int readfile(u64 file, u64 off, u8* mem, u64 len);
int writefile(u64 file, u64 off, u8* mem, u64 len);
int stopfile(int);
//
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);
int openreadclose(void*, int, void*, int);
void printhtmlbody(u8* buf, int len);




struct arena* arena;
struct actor* actor;
void backuper_create(void* addr)
{
	arena = (void*)addr;
	actor = (void*)(addr+0x100000);
}




void scene_export_pix(struct arena* win)
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
void scene_export_vbo(struct arena* win)
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
void scene_export_file(u8* str, int len)
{
	int j;
	struct arena* win;
	for(j=0;j<16;j++)
	{
		win = &arena[j];
		if(_win_ != win->type)continue;

		if(_vbo_ == win->fmt)
		{
			scene_export_vbo(win);
			break;
		}
		if('g' == ((win->fmt&0xff00)>>8))
		{
			scene_export_pix(win);
			break;
		}
	}
}
void scene_export_stdio()
{
	int j;
	struct arena* win;
	struct actor* act;
	say("exporting\n");

	//arena
	say("<arena>\n");
	for(j=0;j<16;j++)
	{
		win = &arena[j];
		if(0 == win->type)break;
		say("    <win type=%.8s fmt=%.8s>\n", &win->type, &win->fmt);
		say("        haha\n");
		say("    </win>\n");
	}
	say("</arena>\n");

	//actor
	say("<actor>\n");
	for(j=0;j<64;j++)
	{
		act = &actor[j];
		if(0 == act->type)break;
		say("    <act type=%.8s name=%.8s>\n", &act->type, &act->name);
		say("        haha\n");
		say("    </act>\n");
	}
	say("</actor>\n");
}




void scene_import_html(u8* name)
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
	j = openreadclose(name, 0, data->buf, 0xf0000);
	if(j <= 0)return;
	say("read:%d\n", j);
	data->len = j;

	printhtmlbody(data->buf, data->len);
}
void scene_import_file(u8* buf, int len)
{
	scene_import_html(buf);
}
void scene_import_stdio()
{
	say("importing\n");
}




void scene(u8* buf, int len)
{
	int j,k;
	u8* argv[8];
	u8 tmp[0x1000];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	k = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(k < 2)return;

	if(0 == ncmp(argv[1], "import", 6))
	{
		if(k == 2)scene_import_stdio();
		else scene_import_file(argv[2], 0);
		return;
	}

	if(0 == ncmp(argv[1], "export", 6))
	{
		if(k == 2)scene_export_stdio();
		else scene_export_file(argv[2], 0);
		return;
	}

	for(j=0;j<k;j++)say("%s\n", argv[j]);
}