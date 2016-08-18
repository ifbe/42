#include<stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#undef main


int width=1024;
int height=768;
unsigned int* mypixel;

SDL_Window* window;//窗口
SDL_Renderer* renderer;	//什么是render
SDL_Texture* texture;		//texture是在显存里的？
SDL_TimerID my_timer_id;


void createwindow(char* addr)
{
	//准备rgb点阵
	mypixel=(unsigned int*)addr;

	//准备sdl
	SDL_Init(SDL_INIT_EVERYTHING);
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

}
void deletewindow()
{
	//释放点阵
	free(mypixel);

	//释放sdl
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window); 
	SDL_Quit(); 
}




//inner
//outer




/*
void point(int x,int y,int color)
{
	mypixel[y*width+x]=color;
}
void draw(int x,int y,int color)
{
	point(x+(width/2),(height/2)-y-1,color);
}*/
QWORD readwindow(QWORD what)
{
}
void writewindow()
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
int uievent(QWORD* type,QWORD* value)
{
	SDL_Event event;
	while (1)
	{
		if (SDL_WaitEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				*type=0;
				return 1;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				*type=1;
				*value=(int)(event.key.keysym.sym);
				return 1;
			}
			else if(event.type == SDL_MOUSEBUTTONDOWN)	//MOUSEMOTION
			{
				if(event.button.button=SDL_BUTTON_LEFT)
				{
					//如果是左键按下的事件，就要判断按下鼠标的时候鼠标是否处在button区域中，所以要得到鼠标坐标
					int x=event.button.x;
					int y=event.button.y;

					*type=2;
					*value=x+(y<<16);
					return 1;
				}
			}
		}//如果有事件
	}//while(1)
}
