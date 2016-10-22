#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#undef main
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void eventwrite(u64,u64);




//
int width=1024;
int height=768;
unsigned int* mypixel;
//sdl
SDL_Window* window;//窗口
SDL_Renderer* renderer;	//什么是render
SDL_Texture* texture;		//texture是在显存里的？
SDL_TimerID my_timer_id;




void windowcreate()
{
	//准备sdl
	SDL_Init(SDL_INIT_EVERYTHING);
}
void windowdelete()
{
	//释放sdl
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window); 
	SDL_Quit(); 
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


//自定义的种类码，不是sdl的不要混淆
//0:退出
//1:键盘按下
//2:键盘松开
//3:鼠标按下
//4:鼠标松开
//5:鼠标移动
//0xff:时间
int uievent(u64* what, u64* who, u64* where, u64* when)
{
	SDL_Event event;
	int val;
	while (1)
	{
		if (SDL_WaitEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				*who = 0;
				return 1;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				*who = 0x64626b;
				val = event.key.keysym.sym;
				//printf("%x\n",val);

				if(val==0x1b){*what=0x1b;return 0;}
				else if(val==0x40000050){*what=0x25;return 0;}
				else if(val==0x40000052){*what=0x26;return 0;}
				else if(val==0x4000004f){*what=0x27;return 0;}
				else if(val==0x40000051){*what=0x28;return 0;}
				else
				{
					*who = 0x72616863;
					*what = val;
					return 1;
				}
			}
			else if(event.type == SDL_MOUSEBUTTONDOWN)	//MOUSEMOTION
			{
				if(event.button.button=SDL_BUTTON_LEFT)
				{
					//如果是左键按下的事件，就要判断按下鼠标的时候鼠标是否处在button区域中，所以要得到鼠标坐标
					int x=event.button.x;
					int y=event.button.y;

					*who = 0x2d6d;
					*what = x+(y<<16)+((u64)1<<48);
					return 1;
				}
			}
		}//如果有事件
	}//while(1)
}
