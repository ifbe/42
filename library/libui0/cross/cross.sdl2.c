#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#undef main
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void eventwrite(u64,u64);




//
static pthread_t id;
static u32* mypixel;
static int width=1024;
static int height=768;
//sdl
static SDL_Window* window;//窗口
static SDL_Renderer* renderer;	//什么是render
static SDL_Texture* texture;		//texture是在显存里的？
static SDL_TimerID my_timer_id;




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
	while(1)
	{
		if(!SDL_WaitEvent(&event))continue;

		if(event.type == SDL_QUIT)
		{
			eventwrite(0,0);
			break;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			int val = event.key.keysym.sym;

			if(val==0x1b)eventwrite(0x1b,0x64626b);
			else if(val==0x40000050)eventwrite(0x25,0x64626b);
			else if(val==0x40000052)eventwrite(0x26,0x64626b);
			else if(val==0x4000004f)eventwrite(0x27,0x64626b);
			else if(val==0x40000051)eventwrite(0x28,0x64626b);
			else if(val==0x8)eventwrite(0x8,0x72616863);
			else if(val==0xd)eventwrite(0xd,0x72616863);
		}
		else if (event.type == SDL_TEXTINPUT)
		{
			int val = event.text.text[0];
			eventwrite(val, 0x72616863);
		}
		else if(event.type == SDL_MOUSEBUTTONDOWN)	//MOUSEMOTION
		{
			if(event.button.button=SDL_BUTTON_LEFT)
			{
				int x=event.button.x;
				int y=event.button.y;
				eventwrite(x+(y<<16)+((u64)1<<48), 0x2d6d);
			}
		}
	}//while(1)

	//释放sdl
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window); 
	SDL_Quit(); 
}




void windowcreate()
{
	//准备sdl
	SDL_Init(SDL_INIT_EVERYTHING);
	pthread_create(&id, NULL, uievent, NULL);
}
void windowdelete()
{
}
void windowstart(char* addr, char* pixfmt, int x, int y)
{
	//准备rgb点阵
	mypixel = (unsigned int*)addr;
	width = x;
	height = y;

	//
	window=SDL_CreateWindow("i am groot!",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				width,height,
				SDL_WINDOW_OPENGL);

	renderer= SDL_CreateRenderer(window, -1, 0);
	texture= SDL_CreateTexture(renderer,
				SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_STREAMING,
				width,height);
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//SDL_RenderClear(renderer);
	//SDL_RenderPresent(renderer);
}
void windowstop()
{
}




void windowread(char* p)
{
}
void windowwrite()
{
  //画texture？
  SDL_UpdateTexture(texture, NULL, mypixel, width*4);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}
