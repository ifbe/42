#include "libuser.h"
void carvesnowman(struct actor* win, u32 rgb, vec3 vc);




void background_json(struct actor* win)
{
	win->len = mysnprintf(win->buf, 0x100000, "{\n");
}
void background_html(struct actor* win)
{
	struct str** ctx = win->ctx;

	ctx[1]->len = mysnprintf(
		ctx[1]->buf, 0x100000,
		"<html><head>\n"
		"<meta charset=\"UTF-8\">\n"
		"<style type=\"text/css\">\n"
		"*{margin:0;padding:0;}\n"
	);
	ctx[2]->len = mysnprintf(
		ctx[2]->buf, 0x100000,
		"<body style=\"width:100%%;height:100%%;\">\n"
	);
}
void background_tui(struct actor* win)
{
}
void background_cli(struct actor* win)
{
}
void preprocess(struct actor* win)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)background_cli(win);
	else if(_tui_ == fmt)background_tui(win);
	else if(_html_ == fmt)background_html(win);
	else if(_json_ == fmt)background_json(win);
}




void foreground_json(struct actor* win)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(buf+len, 0x100000-len, "}\n");

	win->len = len;
}
void foreground_html(struct actor* win)
{
	int len;
	u8* buf;
	struct str** ctx = win->ctx;

	len = ctx[1]->len;
	buf = ctx[1]->buf + len;
	ctx[1]->len += mysnprintf(buf, 0x100000-len, "</style></head>\n");

	len = ctx[2]->len;
	buf = ctx[2]->buf + len;
	ctx[2]->len += mysnprintf(buf, 0x100000-len, "</body></html>\n");
}
void foreground_tui(struct actor* win)
{
}
void foreground_cli(struct actor* win)
{
}
void postprocess(struct actor* win)
{
	u64 fmt = win->fmt;
	if(_cli_ == fmt)foreground_cli(win);
	else if(_tui_ == fmt)foreground_tui(win);
	else if(_html_ == fmt)foreground_html(win);
	else if(_json_ == fmt)foreground_json(win);
}
