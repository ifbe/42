typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#define left 0x1
#define right 0x2
#define up 0x4
#define down 0x8
u32 random_read();




void maze_unblock(u8* data, int w, int h, int x, int y, int mask)
{
	if(left == (mask&left))
	{
		data[w*y+x] &= ~left;
		if(x>0)data[w*y+x-1] &= ~right;
	}
	else if(right == (mask&right))
	{
		data[w*y+x] &= ~right;
		if(x<w-1)data[w*y+x+1] &= ~left;
	}
	else if(up == (mask&up))
	{
		data[w*y+x] &= ~up;
		if(y>0)data[w*(y-1)+x] &= ~down;
	}
	else if(down == (mask&down))
	{
		data[w*y+x] &= ~down;
		if(y<h-1)data[w*(y+1)+x] &= ~up;
	}
}
void maze_block(u8* data, int w, int h, int x, int y, int mask)
{
	if(left == (mask&left))
	{
		data[w*y+x] |= left;
		if(x>0)data[w*y+x-1] |= right;
	}
	else if(right == (mask&right))
	{
		data[w*y+x] |= right;
		if(x<w-1)data[w*y+x+1] |= left;
	}
	else if(up == (mask&up))
	{
		data[w*y+x] |= up;
		if(y>0)data[w*(y-1)+x] |= down;
	}
	else if(down == (mask&down))
	{
		data[w*y+x] |= down;
		if(y<h-1)data[w*(y+1)+x] |= up;
	}
}
int maze_available(u8* data, int w, int h, int x, int y)
{
	u8 count;
	u8 asdf[4];
	u8 this = data[(w*y)+x];

	//random choose one direction
	count = 0;
	if(left == (this&left))
	{
		if(x > 0)
		{
		if((data[(w*y)+x-1]&0x80)==0)
		{
			asdf[count] = left;
			count++;
		}
		}
	}
	if(right == (this&right))
	{
		if(x < w-1)
		{
		if((data[(w*y)+x+1]&0x80)==0)
		{
			asdf[count] = right;
			count++;
		}
		}
	}
	if(up == (this&up))
	{
		if(y > 0)
		{
		if((data[x+(y-1)*w]&0x80)==0)
		{
			asdf[count] = up;
			count++;
		}
		}
	}
	if(down == (this&down))
	{
		if(y < h-1)
		{
		if((data[x+(y+1)*w]&0x80)==0)
		{
			asdf[count] = down;
			count++;
		}
		}
	}
	if(count == 0)return 0;

	count = random_read()%count;
	count = asdf[count];
	return count;
}
void maze_generate_dfs(u8* data, int w, int h, int x, int y)
{
	int tmp;

	//visited -> return
	tmp = (w*y)+x;
	if(0x80 == (data[tmp]&0x80))return;
	data[tmp] |= 0x80;

	//go into
	while(1)
	{
		tmp = maze_available(data, w, h, x, y);
		if(tmp == 0)break;

		maze_unblock(data, w, h, x, y, tmp);
		if(      left == tmp)maze_generate_dfs(data, w, h, x-1, y);
		else if(right == tmp)maze_generate_dfs(data, w, h, x+1, y);
		else if(   up == tmp)maze_generate_dfs(data, w, h, x, y-1);
		else if( down == tmp)maze_generate_dfs(data, w, h, x, y+1);
	}
}
void maze_generate_kruskal()
{
}
void maze_generate_prim()
{
}
void maze_generate(u8* data, int w, int h)
{
	int x,y;
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			//data[y*w+x] = random_read()&0xf;
			data[y*w+x] = 0xf;
		}
	}

	maze_generate_dfs(data, w, h, 0, 0);

	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			data[y*w+x] &= 0xf;
		}
	}
}




void maze_solve_dfs(u8* data, int w, int h)
{
	int x,y,t;
	int sp,tmp;
	u8 xstack[0x1000];
	u8 ystack[0x1000];
	u8 mask[0x1000];

	xstack[0] = 0;
	ystack[0] = 0;
	mask[0] = 0;
	sp = 1;
	while(1)
	{
		if(sp <= 0)break;

		x = xstack[sp-1];
		y = ystack[sp-1];
		if((x == w-1)&&(y == h-1))break;

		t = mask[sp-1];
		tmp = data[(w*y)+x]&0xf;
//say("%d,%d,%d,%d\n",x,y,t,tmp);
		if((0 == (t&left)) && (0 == (tmp&left)) && (x>0))
		{
			mask[sp-1] |= left;

			xstack[sp] = x-1;
			ystack[sp] = y;
			mask[sp] = right;

			sp++;
			continue;
		}
		if((0 == (t&right)) && (0 == (tmp&right)) && (x<w-1))
		{
			mask[sp-1] |= right;

			xstack[sp] = x+1;
			ystack[sp] = y;
			mask[sp] = left;

			sp++;
			continue;
		}
		if((0 == (t&up)) && (0 == (tmp&up)) && (y>0))
		{
			mask[sp-1] |= up;

			xstack[sp] = x;
			ystack[sp] = y-1;
			mask[sp] = down;

			sp++;
			continue;
		}
		if((0 == (t&down)) && (0 == (tmp&down)) && (y<h-1))
		{
			mask[sp-1] |= down;

			xstack[sp] = x;
			ystack[sp] = y+1;
			mask[sp] = up;

			sp++;
			continue;
		}

		sp--;
	}

	for(t=0;t<sp;t++)
	{
		x = xstack[t];
		y = ystack[t];
		data[(w*y)+x] |= 0x80;
	}
}
void maze_solve(u8* data, int w, int h)
{
	maze_solve_dfs(data, w, h);
}