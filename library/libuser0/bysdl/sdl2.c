#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#include"libuser.h"
#undef main
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




void windowread(struct arena* w)
{
	SDL_Event ev;
	SDL_Keysym key;
	u64 why;
	u64 where = (u64)w;
	actorread_all(w);

	SDL_UpdateTexture(w->texture, NULL, w->buf, (w->width)*4);
	SDL_RenderClear(w->er);
	SDL_RenderCopy(w->er, w->texture, NULL, NULL);
	SDL_RenderPresent(w->er);

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
				w->width = w->stride = ev.window.data1;
				w->height = ev.window.data2;

				SDL_DestroyTexture(w->texture);
				w->texture = SDL_CreateTexture(
					w->er,
					SDL_PIXELFORMAT_ARGB8888,
					SDL_TEXTUREACCESS_STREAMING,
					w->width, w->height
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
void windowwrite(struct arena* w)
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
void windowdelete(struct arena* w)
{
	SDL_DestroyTexture(w->texture);
	SDL_DestroyRenderer(w->er);
	SDL_DestroyWindow(w->win); 
	SDL_Quit(); 
}
void windowcreate(struct arena* w)
{
	//data
	w->fmt = hex64('r','g','b','a','8','8','8','8');

	w->width= w->stride = 512;
	w->height = 512;

	w->fbwidth= w->fbstride = 512;
	w->fbheight = 512;

	w->buf = malloc(2048*1024*4);




	//sdl2
	SDL_Init(SDL_INIT_EVERYTHING);

	w->win = SDL_CreateWindow(
		"i am groot!",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		w->width, w->height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	w->er = SDL_CreateRenderer(w->win, -1, 0);
	w->texture = SDL_CreateTexture(
		w->er,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		w->width, w->height
	);
}




void initwindow()
{
}
void freewindow()
{
}
