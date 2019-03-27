#include "libuser.h"
int vkbd_joystick_read(struct arena* cc,void* cf,struct arena* win,void* sty);
int vkbd_joystick_write(struct arena* win,struct event* ev);
int vkbd_keyboard_read(struct arena* cc,void* cf,struct arena* win,void* sty);
int vkbd_keyboard_write(struct arena* win,struct event* ev);




static int vkbd_sread(struct arena* win, struct style* sty, struct arena* cc, struct style* cf)
{/*
	switch(win->vkbdw)
	{
		case 'j':vkbd_joystick_read(0, 0, win, sty);break;
		case 'k':vkbd_keyboard_read(0, 0, win, sty);break;
	}*/
	return 0;
}
static int vkbd_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int ret;
/*
	if(0 == win->vkbdw)return 0;
	ret = 0;
	switch(win->vkbdw)
	{
		case 'j':ret = vkbd_joystick_write(win, ev);break;
		case 'k':ret = vkbd_keyboard_write(win, ev);break;
	}
*/
	return 0;
}
static int vkbd_cread(struct arena* cc, struct style* cf, struct arena* win, struct style* sty)
{
	return 0;
}
static int vkbd_cwrite(struct arena* win, struct style* stack, struct event* ev)
{
	return 0;
}
static int vkbd_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	return 0;
}
static int vkbd_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	return 0;
}
static int vkbd_delete(struct arena* win)
{
	return 0;
}
static int vkbd_create(struct arena* win, u8* str)
{
	return 0;
}




void vkbd_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('v', 'k', 'b', 'd');

	p->oncreate = (void*)vkbd_create;
	p->ondelete = (void*)vkbd_delete;
	p->onstart  = (void*)vkbd_start;
	p->onstop   = (void*)vkbd_stop;
	p->onget    = (void*)vkbd_cread;
	p->onpost   = (void*)vkbd_cwrite;
	p->onread   = (void*)vkbd_sread;
	p->onwrite  = (void*)vkbd_swrite;
}