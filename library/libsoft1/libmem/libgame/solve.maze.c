#include"artery.h"
u32 getrandom();




void maze_generate(u8* data, int size)
{
	int x,y;
	for(y=0;y<size;y++)
	{
		for(x=0;x<size;x++)
		{
			data[y*size+x] = getrandom()%2;
		}
	}
}
void maze_solve()
{
}