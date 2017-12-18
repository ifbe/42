#include"artery.h"
u32 getrandom();




void maze_unblock(u8* data, int size, int x, int y, int mask)
{
	if((mask&1) == 1)
	{
		data[size*y+x] &= ~0x1;
		if(x>0)data[size*y+x-1] &= ~0x2;
	}
	else if((mask&2) == 2)
	{
		data[size*y+x] &= ~0x2;
		if(x<size-1)data[size*y+x+1] &= ~0x1;
	}
	else if((mask&4) == 4)
	{
		data[size*y+x] &= ~0x4;
		if(y>0)data[size*(y-1)+x] &= ~0x8;
	}
	else if((mask&8) == 8)
	{
		data[size*y+x] &= ~0x8;
		if(y<size-1)data[size*(y+1)+x] &= ~0x4;
	}
/*
	else if((mask&0x10) == 0x10)
	{
		data[size*size*z+size*y+x] &= ~0x10;
		if(z>0)data[size*size*(z-1)+size*y+x-1] &= ~0x20;
	}
	else if((mask&0x20) == 0x20)
	{
		data[size*size*z+size*y+x] &= ~0x20;
		if(z<size-1)data[size*size*(z+1)+size*y+x] &= ~0x10;
	}
*/
}




void maze_generate_dfs()
{
}
void maze_generate_kruskal()
{
}
void maze_generate_prim()
{
}
void maze_generate(u8* data, int size)
{
	int x,y;
	for(y=0;y<size;y++)
	{
		for(x=0;x<size;x++)
		{
			//data[y*size+x] = getrandom()&0xf;
			data[y*size+x] = 0xf;
		}
	}
	maze_unblock(data, size, 0, 0, 1);
	maze_unblock(data, size, 2, 3, 1);
	maze_unblock(data, size, 2, 3, 2);
	maze_unblock(data, size, 2, 3, 4);
	maze_unblock(data, size, 2, 3, 8);
}
void maze_solve()
{
}
