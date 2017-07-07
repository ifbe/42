#include "actor.h"
void rectframe(void*,
	int,int,
	int,int,
	u32
);
void navi_create(void*);
void navi_delete();
void navi_read(void*);
void navi_write(void*);
void vkbd_create(void*);
void vkbd_delete();
void vkbd_read(void*);
void vkbd_write(void*);
void vt100_create(void*);
void vt100_delete();
void vt100_read(void*, int, int, int, int);
void vt100_write(void*);





void levitate_read(struct arena* win)
{
	struct relation* rel = win->top;
	rectframe(win,
		(win->w)*((rel->cx)-(rel->wantw/2))/0x10000,
		(win->h)*((rel->cy)-(rel->wanth/2))/0x10000,
		(win->w)*((rel->cx)+(rel->wantw/2))/0x10000,
		(win->h)*((rel->cy)+(rel->wanth/2))/0x10000,
		0xff00ff
	);

	navi_read(win);
	vkbd_read(win);
	vt100_read(win, 0x2000, 0x2000, 0xe000, 0x8000);
}
void levitate_write()
{
}
void levitate_delete()
{
	vt100_delete();
	vkbd_delete();
	navi_delete();
}
void levitate_create(void* base)
{
	navi_create(base);
	vkbd_create(base);
	vt100_create(base);
}