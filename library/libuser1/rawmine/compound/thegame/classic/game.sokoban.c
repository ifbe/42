#include "libuser.h"




static void sokoban_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sokoban_draw_vbo(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sokoban_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sokoban_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sokoban_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sokoban_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sokoban_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)sokoban_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)sokoban_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)sokoban_draw_html(act, pin, win, sty);
	else if(fmt == _json_)sokoban_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)sokoban_draw_vbo(act, pin, win, sty);
	else sokoban_draw_pixel(act, pin, win, sty);
}




static void sokoban_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//sokoban_draw(act, pin, win, sty);
}
static void sokoban_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void sokoban_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void sokoban_start(struct halfrel* self, struct halfrel* peer)
{
}




static void sokoban_search(struct entity* act)
{
}
static void sokoban_modify(struct entity* act)
{
}
static void sokoban_delete(struct entity* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void sokoban_create(struct entity* act)
{
	if(0 == act)return;
	//act->buf = memorycreate(WIDTH*HEIGHT, 0);
}




void sokoban_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'o', 'k', 'o', 'b', 'a', 'n', 0);

	p->oncreate = (void*)sokoban_create;
	p->ondelete = (void*)sokoban_delete;
	p->onsearch = (void*)sokoban_search;
	p->onmodify = (void*)sokoban_modify;

	p->onstart = (void*)sokoban_start;
	p->onstop  = (void*)sokoban_stop;
	p->onread  = (void*)sokoban_read;
	p->onwrite = (void*)sokoban_write;
}
