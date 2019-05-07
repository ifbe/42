#include "libuser.h"




static void sokoban_draw_pixel(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void sokoban_draw_vbo(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void sokoban_draw_json(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void sokoban_draw_html(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void sokoban_draw_tui(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void sokoban_draw_cli(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void sokoban_draw(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)sokoban_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)sokoban_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)sokoban_draw_html(act, pin, win, sty);
	else if(fmt == _json_)sokoban_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)sokoban_draw_vbo(act, pin, win, sty);
	else sokoban_draw_pixel(act, pin, win, sty);
}




static void sokoban_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	sokoban_draw(act, pin, win, sty);
}
static void sokoban_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void sokoban_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void sokoban_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void sokoban_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void sokoban_start(struct halfrel* self, struct halfrel* peer)
{
}
static void sokoban_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void sokoban_create(struct actor* act)
{
	if(0 == act)return;
	//act->buf = memorycreate(WIDTH*HEIGHT);
}




void sokoban_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'o', 'k', 'o', 'b', 'a', 'n', 0);

	p->oncreate = (void*)sokoban_create;
	p->ondelete = (void*)sokoban_delete;
	p->onstart  = (void*)sokoban_start;
	p->onstop   = (void*)sokoban_stop;
	p->oncread  = (void*)sokoban_cread;
	p->oncwrite = (void*)sokoban_cwrite;
	p->onsread  = (void*)sokoban_sread;
	p->onswrite = (void*)sokoban_swrite;
}
