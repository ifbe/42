#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#include"libuser.h"
#undef main
void* arenacreate(u64,u64);




static int alive = 1;
void windowworker(struct arena* w)
{
	SDL_Event event;
	SDL_Init(SDL_INIT_EVERYTHING);

	w->win = SDL_CreateWindow(
		"i am groot!",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		w->width, w->height,
		SDL_WINDOW_OPENGL
	);

	w->er = SDL_CreateRenderer(w->win, -1, 0);

	w->texture = SDL_CreateTexture(
		w->er,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		w->width, w->height
	);

	//SDL_SetRenderDrawColor(w->er, 0, 0, 0, 255);
	//SDL_RenderClear(w->er);
	//SDL_RenderPresent(w->er);

	while(alive)
	{
		actorread_all(w);
		SDL_UpdateTexture(w->texture, NULL, w->buf, (w->width)*4);
		SDL_RenderClear(w->er);
		SDL_RenderCopy(w->er, w->texture, NULL, NULL);
		SDL_RenderPresent(w->er);

		if(!SDL_WaitEvent(&event))continue;
		if(event.type == SDL_QUIT)
		{
			eventwrite(0,0,0,0);
			break;
		}
		else if(event.type == SDL_USEREVENT)
		{
		}
		else if (event.type == SDL_KEYDOWN)
		{
			int val = event.key.keysym.sym;
			//say("val=%x\n",val);

			if(val >= 0xff)
			{
				val &= 0xff;
				if((val>=0x3a)&&(val<=0x45))val += 0xf1-0x3a;
				else if(val==0x52)val = 0x48;
				else if(val==0x50)val = 0x4b;
				else if(val==0x4f)val = 0x4d;
				else if(val==0x51)val = 0x50;
				else continue;

				eventwrite(val,0x64626b,0,0);
			}
			else
			{
				if(val==0x1b)val = 0x1b;
				else if(val==0x8)val = 0x8;
				else if(val==0xd)val = 0xd;
				else if((val>=0x20)&&(val<=0x7e))val = val;
				else continue;

				eventwrite(val, 0x72616863, 0, 0);
			}
		}
/*
		else if (event.type == SDL_TEXTINPUT)
		{
			int val = event.text.text[0];
			if((val>=0x20)&&(val<=0x7e))eventwrite(val, 0x72616863, 0, 0);
		}
*/
		else if(event.type == SDL_MOUSEBUTTONDOWN)	//MOUSEMOTION
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				int x=event.button.x;
				int y=event.button.y;
				eventwrite(
				x+(y<<16)+((u64)'l'<<48),
				0x2d70, 0, 0
				);
			}
		}
	}//while(1)

	//释放sdl
	SDL_DestroyTexture(w->texture);
	SDL_DestroyRenderer(w->er);
	SDL_DestroyWindow(w->win); 
	SDL_Quit(); 
}
void windowthread()
{
	void* win = arenacreate(0, 0);
	windowworker(win);
}
void windowsignal(int sig)
{
	alive = sig;
}




void windowread(char* p)
{
}
void windowwrite()
{
	SDL_Event event;  
	event.type = SDL_USEREVENT;  
	SDL_PushEvent(&event);
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
void windowdelete()
{
}
void windowcreate(struct arena* w)
{
	w->fmt = hex64('r','g','b','a','8','8','8','8');

	w->width= w->stride = 512;
	w->height = 512;

	w->buf = malloc(2048*1024*4);
}




void initwindow()
{
}
void freewindow()
{
}
