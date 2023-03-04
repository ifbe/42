#include <stdio.h>
#include <windows.h>
#include "libuser.h"
struct priv{
    u64 thread;
    int alive;

	HWND hwnd;
	RECT rect;

	HDC hdc1;
	HBITMAP bitmap;
	BITMAPINFO info;

	HDC hdc2;
	unsigned char* buf;
};
static int gdigrab_thread(_obj* win){
    struct priv* my = (struct priv*)win->priv_256b;

	my->hwnd = GetDesktopWindow();
	printf("hwnd=%x\n", my->hwnd);

	GetWindowRect(my->hwnd, &my->rect);
	printf("lt=(%d,%d),rb=(%d,%d)\n", my->rect.left, my->rect.top, my->rect.right, my->rect.bottom);
	int width = my->rect.right - my->rect.left;
	int height= my->rect.bottom - my->rect.top;

	my->hdc1 = GetWindowDC(my->hwnd);
	printf("hdc1=%p\n", my->hdc1);
	my->bitmap = CreateCompatibleBitmap(my->hdc1, width, height);
	printf("bitmap=%p\n", my->bitmap);
	my->info = {{sizeof(BITMAPINFOHEADER), width, height, 1, 24, 0 ,0, 0, 0, 0, 0}, {}};
	printf("info=%p\n", my->info);

	my->hdc2 = CreateCompatibleDC(my->hdc1);
	printf("hdc2=%p\n", my->hdc2);
	SelectObject(my->hdc2, my->bitmap);

	my->buf = (unsigned char*)malloc(width*height*4);
	printf("buf=%p\n", my->buf);

    struct halfrel stack[0x80];
	while(my->alive){
		BitBlt(my->hdc2, 0, 0, width, height, my->hdc1, 0, 0, SRCCOPY);
		GetDIBits(my->hdc2, my->bitmap, 0, height, my->buf, &my->info, DIB_RGB_COLORS);
		//printf("%x,%x,%x,%x\n",my->buf[0],my->buf[1],my->buf[2],my->buf[3]);
        //printmemory(my->buf, 0x20);
        give_data_into_peer(win,_dst_, stack,0, 0,0, my->buf, width*height*3);
	}
	free(my->buf);
	return 0;
}


extern "C" {


int gdigrab_take(_obj* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int gdigrab_give(_obj* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int gdigrab_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gdigrab_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}


int gdigrab_reader(_obj* sup,void* foot, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int gdigrab_writer(_obj* sup,void* foot, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int gdigrab_delete(_obj* win)
{
    struct priv* my = (struct priv*)win->priv_256b;
	my->alive = 0;
	return 0;
}
int gdigrab_create(_obj* win, void* arg, int argc, u8** argv)
{
	int j;
	for(j=1;j<argc;j++){
		arg = argv[j];
		say((void*)"%d->%.16s\n",j,argv[j]);
	}

    struct priv* my = (struct priv*)win->priv_256b;
	my->alive = 1;
	my->thread = threadcreate((void*)gdigrab_thread, win);
	return 0;
}


}	//extern "C"
