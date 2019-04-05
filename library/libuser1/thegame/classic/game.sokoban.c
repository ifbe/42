#include "libuser.h"




static void sokoban_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sokoban_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sokoban_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sokoban_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sokoban_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sokoban_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sokoban_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)sokoban_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)sokoban_read_tui(win, sty, act, pin);
	else if(fmt == _html_)sokoban_read_html(win, sty, act, pin);
	else if(fmt == _json_)sokoban_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)sokoban_read_vbo(win, sty, act, pin);
	else sokoban_read_pixel(win, sty, act, pin);
}
static void sokoban_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void sokoban_cread()
{
}
static void sokoban_cwrite()
{
}
static void sokoban_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void sokoban_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
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
	p->onget    = (void*)sokoban_cread;
	p->onpost   = (void*)sokoban_cwrite;
	p->onread   = (void*)sokoban_sread;
	p->onwrite  = (void*)sokoban_swrite;
}
