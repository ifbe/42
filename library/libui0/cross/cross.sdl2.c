#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#undef main
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
u64* eventread();
void eventwrite(u64,u64,u64,u64);
//
u64 startthread(void*, void*);
void stopthread();
//
void sleep_us(int);
void say(void*, ...);




//
struct sdldata
{
	u64 buf1;
	u64 buf2;
	u64 fmt;
	u64 dim;

	u64 w;
	u64 h;
	u64 d;
	u64 t;

	u64 thread;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};




//自定义的种类码，不是sdl的不要混淆
//0:退出
//1:键盘按下
//2:键盘松开
//3:鼠标按下
//4:鼠标松开
//5:鼠标移动
//0xff:时间
void* uievent(struct sdldata* p)
{
	SDL_Event event;

	//
	p->window =SDL_CreateWindow("i am groot!",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				p->w, p->h,
				SDL_WINDOW_OPENGL);

	p->renderer = SDL_CreateRenderer(p->window, -1, 0);
	p->texture = SDL_CreateTexture(p->renderer,
				SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_STREAMING,
				p->w, p->h);
	//SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);
	//SDL_RenderClear(p->renderer);
	//SDL_RenderPresent(p->renderer);

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
				p->texture, NULL,
				(void*)(p->buf1), (p->w)*4
			);
			SDL_RenderClear(p->renderer);
			SDL_RenderCopy(p->renderer, p->texture, NULL, NULL);
			SDL_RenderPresent(p->renderer);
		}
		else if (event.type == SDL_KEYDOWN)
		{
			int val = event.key.keysym.sym;
			//say("val=%x\n",val);

			if(val==0x1b)eventwrite(0x1b,0x64626b,0,0);
			else if(val==0x40000050)eventwrite(0x25,0x64626b,0,0);
			else if(val==0x40000052)eventwrite(0x26,0x64626b,0,0);
			else if(val==0x4000004f)eventwrite(0x27,0x64626b,0,0);
			else if(val==0x40000051)eventwrite(0x28,0x64626b,0,0);
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
				x+(y<<16)+((u64)1<<48),
				0x2d6d, 0, 0
				);
			}
		}
	}//while(1)

	//释放sdl
	SDL_DestroyTexture(p->texture);
	SDL_DestroyRenderer(p->renderer);
	SDL_DestroyWindow(p->window); 
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
void windowstart(struct sdldata* p)
{
	//准备rgb点阵
	p->buf1 = (u64)malloc(2048*1024*4);

	//
	p->thread = startthread(uievent, p);
}
void windowdelete()
{
}
void windowcreate()
{
	//准备sdl
	SDL_Init(SDL_INIT_EVERYTHING);
}
