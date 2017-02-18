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
static u64 thread;
static u32* mypixel;
static int width=1024;
static int height=768;
//sdl
static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* texture;




//自定义的种类码，不是sdl的不要混淆
//0:退出
//1:键盘按下
//2:键盘松开
//3:鼠标按下
//4:鼠标松开
//5:鼠标移动
//0xff:时间
void* uievent(void* p)
{
	SDL_Event event;

	//
	window =SDL_CreateWindow("i am groot!",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				width,height,
				SDL_WINDOW_OPENGL);

	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer,
				SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_STREAMING,
				width,height);
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//SDL_RenderClear(renderer);
	//SDL_RenderPresent(renderer);

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
			SDL_UpdateTexture(texture, NULL, mypixel, width*4);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
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
			if(event.button.button=SDL_BUTTON_LEFT)
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
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window); 
	SDL_Quit(); 
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
void windowstart(char* addr, char* pixfmt, int x, int y)
{
	//准备rgb点阵
	mypixel = (unsigned int*)addr;
	width = x;
	height = y;

	thread = startthread(uievent, 0);
	sleep_us(50*1000);
}
void windowdelete()
{
}
void windowcreate()
{
	//准备sdl
	SDL_Init(SDL_INIT_EVERYTHING);
}