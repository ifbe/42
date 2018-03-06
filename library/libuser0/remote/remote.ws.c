#include"arena.h"
//
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);
//
int websocket_read(void*, int);
int websocket_write(u64, u8*, int);




/*
	len = websocket_read(buf, len);
	if(len < 0)goto theend;

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
}




int wsserver_start(struct window* win)
{
}
int wsserver_write(struct window* win)
{
	if(0 == win)return 0;
	say("@wsserverwrite: fd=%x\n", win->fd);
	websocket_write(win->fd, "fuck", 4);
}