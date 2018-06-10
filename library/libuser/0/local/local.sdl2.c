#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#include"arena.h"
#undef main




static struct arena* src;




void* uievent(struct arena* this)
{
	SDL_Event event;
	this->win = SDL_CreateWindow(
		"i am groot!",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		this->w, this->h,
		SDL_WINDOW_OPENGL
	);

	this->er = SDL_CreateRenderer(this->win, -1, 0);

	this->texture = SDL_CreateTexture(
		this->er,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		this->w, this->h
	);

	//SDL_SetRenderDrawColor(this->er, 0, 0, 0, 255);
	//SDL_RenderClear(this->er);
	//SDL_RenderPresent(this->er);

	while(1)
	{
		if(!SDL_WaitEvent(&event))continue;

		if(event.type == SDL_QUIT)
		{
			eventwrite(0,0,0,0);
			break;
		}
		else if(event.type == SDL_USEREVENT)
		{
			SDL_UpdateTexture(
				this->texture, NULL,
				src->buf, (this->w)*4
			);
			SDL_RenderClear(this->er);
			SDL_RenderCopy(this->er, this->texture, NULL, NULL);
			SDL_RenderPresent(this->er);
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
	SDL_DestroyTexture(this->texture);
	SDL_DestroyRenderer(this->er);
	SDL_DestroyWindow(this->win); 
	SDL_Quit(); 
	return 0;
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
void windowstart(struct arena* this)
{
	src = this;
	this->w = 512;
        this->h = 512;
	startthread(uievent, this);
}
void windowdelete()
{
}
void windowcreate()
{
}




void initwindow()
{
	SDL_Init(SDL_INIT_EVERYTHING);
}
void freewindow()
{
}
