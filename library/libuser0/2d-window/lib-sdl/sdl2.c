#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#include"libuser.h"
#undef main
void* supply_alloc();
void* supply_recycle(void*);
int rgbanode_take(void*,void*, void*,int, void*,int, void*,int);
int rgbanode_give(void*,void*, void*,int, void*,int, void*,int);




static u8 uppercase[] = {
	' ', '!','\"', '#', '$', '%', '&','\"',		//20,27
	'(', ')', '*', '+', '<', '_', '>', '?',		//28,2f
	')', '!', '@', '#', '$', '%', '^', '&',		//30,37
	'*', '(', ':', ':', '<', '+', '>', '?',		//38,3f
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',		//40,47
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',		//48,4f
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',		//50,57
	'X', 'Y', 'Z', '{', '|', '}', '^', '_',		//58,5f
	'~', 'A', 'B', 'C', 'D', 'E', 'F', 'G',		//60,67
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',		//68,6f
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',		//70,77
	'X', 'Y', 'Z', '{', '|', '}', '~', ' ',		//78,7f
};




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	SDL_Event ev;
	SDL_Keysym sym;
	struct event msg;

	//read context
	rgbanode_take(wnd,0, stack,sp, 0,0, 0,0);

	//per sdlwnd
	struct rgbasdl* per = &wnd->rgbasdl;

	//update screen
	SDL_UpdateTexture(per->sdltex, NULL, per->buf, (wnd->whdf.width)*4);
	SDL_RenderClear(per->sdlren);
	SDL_RenderCopy(per->sdlren, per->sdltex, NULL, NULL);
	SDL_RenderPresent(per->sdlren);

	//cleanup events
	msg.where = (u64)wnd;
	while(SDL_PollEvent(&ev))
	{
		if(SDL_QUIT == ev.type)
		{
			eventwrite(0,0,0,0);
			return;
		}
		else if(SDL_USEREVENT == ev.type)
		{
		}
		else if(SDL_WINDOWEVENT == ev.type)
		{
			if(SDL_WINDOWEVENT_RESIZED == ev.window.event)
			{
				//logtoall("%d,%d\n", ev.window.data1, ev.window.data2);
				wnd->whdf.width = ev.window.data1;
				wnd->whdf.height = ev.window.data2;

				wnd->whdf.fbwidth = wnd->whdf.width*4;
				//wnd->whdf.fbheight = 0;

				SDL_DestroyTexture(per->sdltex);
				per->sdltex = SDL_CreateTexture(
					per->sdlren,
					SDL_PIXELFORMAT_ABGR8888,
					SDL_TEXTUREACCESS_STREAMING,
					wnd->whdf.width, wnd->whdf.height
				);
			}
		}
		else if (SDL_KEYDOWN == ev.type)
		{
			sym = ev.key.keysym;
			msg.why = sym.sym;
			//logtoall("%llx\n",msg.why);

			if(msg.why >= 0xff)
			{
				msg.why &= 0xff;
				if((msg.why>=0x3a)&&(msg.why<=0x45))msg.why += 0xf1-0x3a;
				else if(msg.why==0x52)msg.why = 0x48;
				else if(msg.why==0x50)msg.why = 0x4b;
				else if(msg.why==0x4f)msg.why = 0x4d;
				else if(msg.why==0x51)msg.why = 0x50;
				else continue;

				msg.what = _kbd_;
				rgbanode_give(wnd,0, stack,sp, 0,0, &msg,0);
			}
			else
			{
				if((msg.why >= 0x20)&&(msg.why <= 0x7f))
				{
					if((sym.mod & KMOD_LSHIFT) | (sym.mod & KMOD_CAPS))
					{
						msg.why = uppercase[msg.why-0x20];
					}
				}
				msg.what = _char_;
				rgbanode_give(wnd,0, stack,sp, 0,0, &msg,0);
			}
		}
		else if(SDL_MOUSEWHEEL == ev.type)
		{
			short* t = (void*)&msg.why;
			t[0] = ev.button.x;
			t[1] = ev.button.y;
			t[2] = ev.wheel.y;
			t[3] = (ev.wheel.y > 0) ? 'f' : 'b';

			msg.what = 0x4070;
			rgbanode_give(wnd,0, stack,sp, 0,0, &msg,0);
		}
		else if(SDL_MOUSEBUTTONDOWN == ev.type)
		{
			//logtoall("+%d,%d\n", ev.button.x, ev.button.y);
			if(SDL_BUTTON_LEFT == ev.button.button)
			{
				int x = ev.button.x;
				int y = ev.button.y;
				msg.why = x+(y<<16)+((u64)'l'<<48);
				msg.what = 0x2b70;
				rgbanode_give(wnd,0, stack,sp, 0,0, &msg,0);
			}
		}
		else if(SDL_MOUSEBUTTONUP == ev.type)
		{
			//logtoall("-%d,%d\n", ev.button.x, ev.button.y);
			if(SDL_BUTTON_LEFT == ev.button.button)
			{
				int x = ev.button.x;
				int y = ev.button.y;
				msg.why = x+(y<<16)+((u64)'l'<<48);
				msg.what = 0x2d70;
				rgbanode_give(wnd,0, stack,sp, 0,0, &msg,0);
			}
		}
		else if(SDL_MOUSEMOTION == ev.type)
		{
			//logtoall("@%d,%d\n", ev.button.x, ev.button.y);
			int x = ev.button.x;
			int y = ev.button.y;
			msg.why = x+(y<<16)+((u64)'l'<<48);
			msg.what = 0x4070;
			msg.when = timeread_us();
			rgbanode_give(wnd,0, stack,sp, 0,0, &msg,0);
		}
	}
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//SDL_Event ev;  
	//ev.type = SDL_USEREVENT;  
	//SDL_PushEvent(&ev);
}
void window_attach()
{
}
void window_detach()
{
}




void window_read()
{
}
void window_write()
{
}
void window_delete(_obj* wnd)
{
	//per sdlwnd
	struct rgbasdl* per = &wnd->rgbasdl;

	SDL_DestroyTexture(per->sdltex);
	SDL_DestroyRenderer(per->sdlren);
	SDL_DestroyWindow(per->sdlwnd);
	SDL_Quit(); 
}
void window_create(_obj* wnd)
{
	//data
	wnd->vfmt = _rgba8888_;

	wnd->whdf.width= 1024;
	wnd->whdf.height = 768;

	wnd->whdf.fbwidth = 1024*4;
	//wnd->whdf.fbheight = 768;




	//per sdlwnd
	struct rgbasdl* per = &wnd->rgbasdl;

	per->buf = malloc(2048*2048*4);




	//sdl2
	SDL_Init(SDL_INIT_EVERYTHING);

	per->sdlwnd = SDL_CreateWindow(
		"i am groot!",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		wnd->whdf.width, wnd->whdf.height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	per->sdlren = SDL_CreateRenderer(per->sdlwnd, -1, 0);
	per->sdltex = SDL_CreateTexture(
		per->sdlren,
		SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		wnd->whdf.width, wnd->whdf.height
	);
}




void* window_alloc()
{
	return supply_alloc();
}




void initwindow()
{
}
void freewindow()
{
}
