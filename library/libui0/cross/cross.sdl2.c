#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#include"arena.h"
#undef main




static struct window* src;




void* uievent(struct window* this)
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

			if(val==0x1b)eventwrite(0x1b,0x64626b,0,0);
			else if(val==0x4000003a)eventwrite(0xf1,0x64626b,0,0);
			else if(val==0x4000003b)eventwrite(0xf2,0x64626b,0,0);
			else if(val==0x4000003c)eventwrite(0xf3,0x64626b,0,0);
			else if(val==0x4000003d)eventwrite(0xf4,0x64626b,0,0);
			else if(val==0x40000052)eventwrite(0x48,0x64626b,0,0);
			else if(val==0x40000050)eventwrite(0x4b,0x64626b,0,0);
			else if(val==0x4000004f)eventwrite(0x4d,0x64626b,0,0);
			else if(val==0x40000051)eventwrite(0x50,0x64626b,0,0);
			else if(val==0x8)eventwrite(0x8,0x72616863,0,0);
			else if(val==0xd)eventwrite(0xd,0x72616863,0,0);
			else if((val>=0x20)&&(val<=0x7e))eventwrite(val, 0x72616863, 0, 0);
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
void windowstart(struct window* this)
{
	this->w = 512;
        this->h = 512;
        if(this->type == hex32('b','u','f',0))
	{
		src = this;
	}
	else
	{
		this->thread = startthread(uievent, this);
	}
}
void windowdelete()
{
}
void windowcreate()
{
	//准备sdl
	SDL_Init(SDL_INIT_EVERYTHING);
}
