#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#include"libuser.h"
#undef main
int rgbanode_read(void*, void*);
int rgbanode_write(void*, void*);




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




void windowread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	SDL_Event ev;
	SDL_Keysym key;
	u64 why;
	u64 where;
	struct supply* wnd = self->pchip;

	//read context
	rgbanode_read(wnd, 0);

	//update screen
	SDL_UpdateTexture(wnd->sdltex, NULL, wnd->rgbabuf, (wnd->width)*4);
	SDL_RenderClear(wnd->sdlren);
	SDL_RenderCopy(wnd->sdlren, wnd->sdltex, NULL, NULL);
	SDL_RenderPresent(wnd->sdlren);

	//cleanup events
	where = (u64)wnd;
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
				//say("%d,%d\n", ev.window.data1, ev.window.data2);
				wnd->width = ev.window.data1;
				wnd->height = ev.window.data2;

				wnd->fbwidth = wnd->width*4;
				//wnd->fbheight = 0;

				SDL_DestroyTexture(wnd->sdltex);
				wnd->sdltex = SDL_CreateTexture(
					wnd->sdlren,
					SDL_PIXELFORMAT_ARGB8888,
					SDL_TEXTUREACCESS_STREAMING,
					wnd->width, wnd->height
				);
			}
		}
		else if (SDL_KEYDOWN == ev.type)
		{
			key = ev.key.keysym;
			why = key.sym;
			//say("%llx\n",why);

			if(why >= 0xff)
			{
				why &= 0xff;
				if((why>=0x3a)&&(why<=0x45))why += 0xf1-0x3a;
				else if(why==0x52)why = 0x48;
				else if(why==0x50)why = 0x4b;
				else if(why==0x4f)why = 0x4d;
				else if(why==0x51)why = 0x50;
				else continue;

				eventwrite(why, _kbd_, where, 0);
			}
			else
			{
				if((why >= 0x20)&&(why <= 0x7f))
				{
					if((key.mod & KMOD_LSHIFT) | (key.mod & KMOD_CAPS))
					{
						why = uppercase[why-0x20];
					}
				}
				eventwrite(why, _char_, where, 0);
			}
		}
		else if(SDL_MOUSEWHEEL == ev.type)
		{
			if(ev.wheel.y > 0) // scroll up
			{
				why = 'f';
			}
			else if(ev.wheel.y < 0) // scroll down
			{
				why = 'b';
			}
			int x = ev.button.x;
			int y = ev.button.y;
			why = x+(y<<16)+(why<<48);
			eventwrite(why, 0x2b70, where, 0);
		}
		else if(SDL_MOUSEBUTTONDOWN == ev.type)
		{
			//say("+%d,%d\n", ev.button.x, ev.button.y);
			if(SDL_BUTTON_LEFT == ev.button.button)
			{
				int x = ev.button.x;
				int y = ev.button.y;
				why = x+(y<<16)+((u64)'l'<<48);
				eventwrite(why, 0x2b70, where, 0);
			}
		}
		else if(SDL_MOUSEBUTTONUP == ev.type)
		{
			//say("-%d,%d\n", ev.button.x, ev.button.y);
			if(SDL_BUTTON_LEFT == ev.button.button)
			{
				int x = ev.button.x;
				int y = ev.button.y;
				why = x+(y<<16)+((u64)'l'<<48);
				eventwrite(why, 0x2d70, where, 0);
			}
		}
		else if(SDL_MOUSEMOTION == ev.type)
		{
			//say("@%d,%d\n", ev.button.x, ev.button.y);
			int x = ev.button.x;
			int y = ev.button.y;
			why = x+(y<<16)+((u64)'l'<<48);
			eventwrite(why, 0x4070, where, 0);
		}
	}
}
void windowwrite(struct supply* wnd)
{
	//SDL_Event ev;  
	//ev.type = SDL_USEREVENT;  
	//SDL_PushEvent(&ev);
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(struct supply* wnd)
{
	SDL_DestroyTexture(wnd->sdltex);
	SDL_DestroyRenderer(wnd->sdlren);
	SDL_DestroyWindow(wnd->sdlwnd); 
	SDL_Quit(); 
}
void windowcreate(struct supply* wnd)
{
	//data
	wnd->fmt = _rgba_;
	wnd->vfmt = hex64('r','g','b','a','8','8','8','8');

	wnd->width= 1024;
	wnd->height = 768;

	wnd->fbwidth = 1024*4;
	//wnd->fbheight = 768;

	wnd->rgbabuf = malloc(2048*2048*4);




	//sdl2
	SDL_Init(SDL_INIT_EVERYTHING);

	wnd->sdlwnd = SDL_CreateWindow(
		"i am groot!",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		wnd->width, wnd->height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	wnd->sdlren = SDL_CreateRenderer(wnd->sdlwnd, -1, 0);
	wnd->sdltex = SDL_CreateTexture(
		wnd->sdlren,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		wnd->width, wnd->height
	);
}




void initwindow()
{
}
void freewindow()
{
}
