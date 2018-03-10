#include"arena.h"
//
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);
//
int websocket_read(void*, int);
int websocket_write(u64, u8*, int);
//
void* arteryread(int fd);




static void* buffer = 0;
/*

	ret = obj[fd].stage1;
	if(ret == 1)
	{
		say("%.*s\n", len, buf);

		websocket_write(fd, "four two", 8);
		obj[fd].stage1 = 2;
	}
	else if(ret == 2)
	{
		say("%.*s\n", len, buf);

		websocket_write(fd, "haha@2", 6);
		obj[fd].stage1 = 3;
	}
	else if(ret == 3)
	{
		printmemory(buf, len);
		websocket_write(fd, "success", 7);

		obj[fd].stage1 = 4;
	}

int ws_event(struct event* ev, void* buf)
{
	if(ncmp(buf, "kbd ", 4) == 0)
	{
		ev->what = hex32('k','b','d',0);
		decstr2data(buf+4, &(ev->why));
	}
	else if(ncmp(buf, "char ", 5) == 0)
	{
		ev->what = hex32('c','h','a','r');
		decstr2data(buf+5, &(ev->why));
	}
	return 0;
}*/
int wsclient_start(struct window* win)
{
	return 0;
}




int wsserver_start(struct window* win)
{
	if(0 == buffer)buffer = startmemory(0x100000);

	win->buf = buffer;
	return 0;
}
int wsserver_stop(struct window* win)
{
	return 0;
}
int wsserver_read(struct window* win)
{
	int len;
	struct window* w = arteryread(win->fd);

	len = websocket_read(w->buf, w->len);
	if(len < 0)return 0;
	if(len > 0x100000)len = 0x100000;

	say("%.*s\n", len, w->buf);
	return 0;
}
int wsserver_write(struct window* win)
{
	if(0 == win)return 0;
	if(0 == win->buf)return 0;
	if(0 == win->len)
	{
		win->len = 4;
		*(u32*)(win->buf) = hex32('h','a','h','a');
	}
	//say("@wsserverwrite: fd=%x\n", win->fd);

	websocket_write(win->fd, win->buf, win->len);
	return 0;
}
