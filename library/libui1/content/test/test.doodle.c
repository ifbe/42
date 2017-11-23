#include "actor.h"
//
void drawline(void*, int ax, int ay, int bx, int by, u32 color);
void drawline_bezier(void*, int ax, int ay, int bx, int by, int cx, int cy, u32 color);
void drawline_rect(void*, int x1, int y1, int x2, int y2, u32 color);
void drawline_circle(void*, int cx, int cy, int r, u32 color);
void drawsolid_rect(void*, int x1, int y1, int x2, int y2, u32 color);
void drawsolid_circle(void*, int cx, int cy, int r, u32 color);
void carvepoint_cone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz);
void carveline_cone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz);
void carvesolid_cone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz);
void carvepoint_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz);
void carvepoint_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz);
void carvepoint_dodecahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz);
void carveline_dodecahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz);
void carvesolid_dodecahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz);
void carvepoint_icosahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz);
void carveline_icosahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz);
void carvesolid_icosahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz);



//
static int px=0,py=0;




void doodle_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	//rect
	drawsolid_rect(win, 10, 10, 90, 90, 0xff00);
	drawline_rect(win, 110, 10, 190,90, 0xff00ff);

	//circle
	drawsolid_circle(win, 50, 150, 40, 0xff);
	drawline_circle(win, 150, 150, 40, 0xff0000);

	//moon
	drawsolid_circle(win, 50, 250, 40, 0xffff00);
	drawsolid_circle(win, 40, 240, 40, 0);

	//bezier
	drawline(win, 0, 256,  px,  py, 0xffff);
	drawline(win, 512, 256,  px,  py, 0xffff);
	drawline_bezier(win, 0, 256, 512, 256, px, py, 0xffff);
}
static void doodle_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;

	carvesolid_icosahedron(
		win, 0xffffff,
		cx, cy, 0.0,
		w/2, 0.0, 0.0,
		0.0, w/2, 0.0,
		0.0, 0.0, w/2
	);
}
static void doodle_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void doodle_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void doodle_read_cli()
{
}
static void doodle_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == hex32('c','l','i',0))doodle_read_cli();
	else if(fmt == hex32('t','u','i',0))doodle_read_tui(win, act, sty);
	else if(fmt == hex32('h','t','m','l'))doodle_read_html(win, act, sty);
	else if(fmt == hex32('v','b','o',0))doodle_read_vbo(win, act, sty);
	else doodle_read_pixel(win, act, sty);
}
void doodle_write(struct event* ev)
{
	u64 what = ev->what;
	u64 why = ev->why;
	if(what == 0x2d70)
	{
		px = why & 0xffff;
		py = (why >> 16) & 0xffff;
	}
}
void doodle_list()
{
}
void doodle_change()
{
}
void doodle_start()
{
}
void doodle_stop()
{
}
void doodle_create(void* base, void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('d', 'o', 'o', 'd', 'l', 'e', 0, 0);

	p->start = (void*)doodle_start;
	p->stop = (void*)doodle_stop;
	p->list = (void*)doodle_list;
	p->choose = (void*)doodle_change;
	p->read = (void*)doodle_read;
	p->write = (void*)doodle_write;
}
void doodle_delete()
{
}
