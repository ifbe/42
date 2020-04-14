#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#include"libuser.h"
#undef main
int rgbanode_read(void*,int, void*,int, void*,int, void*,int);
int rgbanode_write(void*,int, void*,int, void*,int, void*,int);




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




void windowread(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	SDL_Event ev;
	SDL_Keysym sym;
	struct event msg;

	//read context
	rgbanode_read(wnd,0, stack,sp, 0,0, 0,0);

	//update screen
	SDL_UpdateTexture(wnd->sdltex, NULL, wnd->rgbabuf, (wnd->width)*4);
	SDL_RenderClear(wnd->sdlren);
	SDL_RenderCopy(wnd->sdlren, wnd->sdltex, NULL, NULL);
	SDL_RenderPresent(wnd->sdlren);

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
				//say("%d,%d\n", ev.window.data1, ev.window.data2);
				wnd->width = ev.window.data1;
				wnd->height = ev.window.data2;

				wnd->fbwidth = wnd->width*4;
				//wnd->fbheight = 0;

				SDL_DestroyTexture(wnd->sdltex);
				wnd->sdltex = SDL_CreateTexture(
					wnd->sdlren,
					SDL_PIXELFORMAT_ABGR8888,
					SDL_TEXTUREACCESS_STREAMING,
					wnd->width, wnd->height
				);
			}
		}
		else if (SDL_KEYDOWN == ev.type)
		{
			sym = ev.key.keysym;
			msg.why = sym.sym;
			//say("%llx\n",msg.why);

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
				rgbanode_write(wnd,0, stack,sp, 0,0, &msg,0);
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
				rgbanode_write(wnd,0, stack,sp, 0,0, &msg,0);
			}
		}
		else if(SDL_MOUSEWHEEL == ev.type)
		{
			if(ev.wheel.y > 0) // scroll up
			{
				msg.why = 'f';
			}
			else if(ev.wheel.y < 0) // scroll down
			{
				msg.why = 'b';
			}
			int x = ev.button.x;
			int y = ev.button.y;
			msg.why = x+(y<<16)+(msg.why<<48);
			msg.what = 0x2b70;
			rgbanode_write(wnd,0, stack,sp, 0,0, &msg,0);
		}
		else if(SDL_MOUSEBUTTONDOWN == ev.type)
		{
			//say("+%d,%d\n", ev.button.x, ev.button.y);
			if(SDL_BUTTON_LEFT == ev.button.button)
			{
				int x = ev.button.x;
				int y = ev.button.y;
				msg.why = x+(y<<16)+((u64)'l'<<48);
				msg.what = 0x2b70;
				rgbanode_write(wnd,0, stack,sp, 0,0, &msg,0);
			}
		}
		else if(SDL_MOUSEBUTTONUP == ev.type)
		{
			//say("-%d,%d\n", ev.button.x, ev.button.y);
			if(SDL_BUTTON_LEFT == ev.button.button)
			{
				int x = ev.button.x;
				int y = ev.button.y;
				msg.why = x+(y<<16)+((u64)'l'<<48);
				msg.what = 0x2d70;
				rgbanode_write(wnd,0, stack,sp, 0,0, &msg,0);
			}
		}
		else if(SDL_MOUSEMOTION == ev.type)
		{
			//say("@%d,%d\n", ev.button.x, ev.button.y);
			int x = ev.button.x;
			int y = ev.button.y;
			msg.why = x+(y<<16)+((u64)'l'<<48);
			msg.what = 0x4070;
			rgbanode_write(wnd,0, stack,sp, 0,0, &msg,0);
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
		SDL_PIXELFORMAT_ABGR8888,
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
