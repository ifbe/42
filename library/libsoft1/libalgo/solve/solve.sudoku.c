#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int




int sudoku_next(u8 (*data)[9], u8 (*stack)[9], int x, int y)
{/*
	int j,k;
	int basex,basey;
	u8 val[10];

	for(j=0;j<10;j++)val[j] = 0;
	basex = x - (x%3);
	basey = y - (y%3);
	for(j=0;j<9;j++)
	{
		k = data[y][j];
		if(0 != k)val[k] = 1;

		k = data[j][x];
		if(0 != k)val[k] = 1;

		k = data[basey+(j/3)][basex+(j%3)];
		if(0 != k)val[k] = 1;
	}

	k = stack[y][x];
	for(j=k+1;j<10;j++)
	{
		if(0 == val[j])
		{
			data[y][x] = j;
			stack[y][x] = j;
			//say("k=%d,j=%d\n", k, j);
			break;
		}
	}*/

	u8 val[10];
	int j,k,basex,basey;
	for(j=1;j<10;j++)val[j] = 0;

	//test in orig
	basex = x - (x%3);
	basey = y - (y%3);
	for(j=0;j<9;j++)
	{
		k = data[y][j];
		if(k < 10)val[k] = 1;

		k = data[j][x];
		if(k < 10)val[k] = 1;

		k = data[basey+(j/3)][basex+(j%3)];
		if(k < 10)val[k] = 1;
	}

	//test in stack
	for(k=stack[y][x]+1;k<10;k++)
	{
		if(1 == val[k])continue;

		basex = x - (x%3);
		basey = y - (y%3);
		for(j=0;j<9;j++)
		{
			if(k == stack[y][j])
			{
				if(x != j)break;
			}

			if(k == stack[j][x])
			{
				if(y != j)break;
			}

			if(k == stack[basey+(j/3)][basex+(j%3)])
			{
				if(x != basex+(j%3))break;
				if(y != basey+(j/3))break;
			}
		}
		if(j >= 9)break;	//success
	}
	return k;
}
void sudoku_solve(u8 (*data)[9])
{
	int x,y;
	int t,ret;
	//u8 haha[9][9];
	u8 stack[9][9];

	//prepare
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)stack[y][x] = data[y][x];
	}

	t = 0;
	while(1)
	{
begin:
//printmemory(stack, t);
		y = t/9;
		x = t%9;

		if(0 == data[y][x])
		{
			ret = sudoku_next(data, stack, x, y);
			if(ret < 10)	//success
			{
				stack[y][x] = ret;
				goto next;
			}
			else	//fail
			{
				stack[y][x] = 0;
				while(1)
				{
					t--;
					if(t < 0)return;

					y = t/9;
					x = t%9;
					if(0 == data[y][x])goto begin;
				}
			}
		}
next:
		t++;
		if(t >= 81)break;
	}
/*
	//backtrack
	t = 0;
	while(1)
	{
		say("t=%d\n",t);
		//printmemory(haha, t);
		//printmemory(stack, t);
		y = t/9;
		x = t%9;
		if(0 == haha[y][x])
		{
			sudoku_try(haha, stack, x, y);
			if(0 == haha[y][x])
			{
				stack[y][x] = 0;
				while(1)
				{
					t--;
					if(t < 0)return;

					y = t/9;
					x = t%9;
					if(0 == data[y][x])
					{
						haha[y][x] = 0;
						break;
					}
				}
				continue;
			}
		}

		t++;
		if(t >= 81)break;
	}
	if(t < 81)return;
*/
	//success
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)data[y][x] = stack[y][x];
	}
}
void sudoku_generate(u8 (*data)[9])
{
	int x,y;
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)data[y][x] = 0;
	}
}