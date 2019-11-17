#include "libuser.h"




void background_html(struct actor* win)
{
	struct str** ctx = win->ctx;

	ctx[0]->len = mysnprintf(
		ctx[0]->buf, 0x10000,
		"<!DOCTYPE html>\n"
		"<html>\n"
	);
	ctx[1]->len = mysnprintf(
		ctx[1]->buf, 0x10000,
		"<head>\n"
		"<meta charset=\"UTF-8\">\n"
		"<style type=\"text/css\">\n"
		"*{margin:0;padding:0;}\n"
		"body{width:100%%;height:100%%;background-color:#808080;}\n"
	);
	ctx[2]->len = mysnprintf(
		ctx[2]->buf, 0x10000,
		"<body>\n"
	);
}
void foreground_html(struct actor* win)
{
	int len;
	u8* buf;
	struct str** ctx = win->ctx;

	len = ctx[1]->len;
	buf = ctx[1]->buf + len;
	ctx[1]->len += mysnprintf(buf, 0x10000-len, "</style>\n</head>\n");

	len = ctx[2]->len;
	buf = ctx[2]->buf + len;
	ctx[2]->len += mysnprintf(buf, 0x10000-len, "</body>\n");

	ctx[3]->len = mysnprintf(
		ctx[3]->buf, 0x10000,
		"</html>"
	);
}




int htmlnode_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void** buf, int len)
{
	int j;
	struct actor* win;
	struct str** ctx;
	say("@htmlnode_read\n");

	win = self->pchip;
	background_html(win);
	foreground_html(win);

	ctx = win->ctx;
	for(j=0;j<4;j++)buf[j] = ctx[j];
	return 0;
}
int htmlnode_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@htmlnode_write\n");
	relationwrite(self->pchip, _src_, "text/html", 0, "htmlnode_read\n", 14);
	return 0;
}
int htmlnode_delete(struct actor* win)
{
	return 0;
}
int htmlnode_create(struct actor* win, void* str)
{
	void** ctx = memorycreate(0x1000, 0);
	void*  buf = memorycreate(0x100000, 0);

	ctx[0] = buf+0x00000;
	ctx[1] = buf+0x10000;
	ctx[2] = buf+0x20000;
	ctx[3] = buf+0x30000;

	win->ctx = ctx;
	win->buf = buf;
	return 0;
}
