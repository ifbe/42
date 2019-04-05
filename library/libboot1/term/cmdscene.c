#include "libuser.h"
int startfile(void*, int);
int readfile(u64 file, u64 off, u8* mem, u64 len);
int writefile(u64 file, u64 off, u8* mem, u64 len);
int stopfile(int);
//
u8* getsuffix(u8* p);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);
int openreadclose(void*, int, void*, int);




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




void exportactor(struct actor* act)
{
	say("/%.8s/%.8s/%.8s\n", &act->tier, &act->type, &act->fmt);
}
void exportarena(struct arena* win)
{
	struct relation* rel;
	struct arena* tmp;
	struct style* sty;
	struct actor* act;
	say("/%.8s/%.8s/%.8s/%.8s{\n", &win->tier, &win->type, &win->fmt, &win->vfmt);

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;
		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty)
			{
				say("%f,%f,%f,%x\n",sty->vc[0],sty->vc[1],sty->vc[2],sty->wc);
				say("%f,%f,%f,%x\n",sty->vr[0],sty->vr[1],sty->vr[2],sty->wr);
				say("%f,%f,%f,%x\n",sty->vf[0],sty->vf[1],sty->vf[2],sty->wf);
				say("%f,%f,%f,%x\n",sty->vu[0],sty->vu[1],sty->vu[2],sty->wu);
			}

			act = (void*)(rel->dstchip);
			exportactor(act);
		}
		if(_win_ == rel->dsttype)
		{
			tmp = (void*)(rel->dstchip);
			exportarena(tmp);
		}
		rel = samesrcnextdst(rel);
	}

	say("}\n");
}
void scene_export_stdio()
{
	int j;
	struct arena* win;
	say("exporting\n");

	for(j=0;j<16;j++)
	{
		win = &arena[j];
		if(_win_ == win->type)exportarena(win);
	}
/*	int j;
	struct arena* win;
	struct actor* act;

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
	say("</actor>\n");*/
}




void scene_import_file(u8* buf, int len)
{
	u8* suf = getsuffix(buf);
	if(0 == suf)return;

	if(0 == ncmp(suf, "htm", 3))arenacreate(_html_, buf);
	if(0 == ncmp(suf, "sch", 3))arenacreate(_sch_ , buf);
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