unsigned int getrandom();

int sudoku_random(char* p, int len)
{
	int k,ret;
	if(len == 0)return 0;

	k = getrandom()%len;
	ret = p[k];
	if(len > 1)
	{
		p[k] = p[len-1];
		p[len-1] = 0;
	}

	return ret;
}
int sudoku_possiable(char (*table)[9], char* p, int px, int py)
{
	int x,y,ret;
	int basex, basey;
	for(x=0;x<10;x++)p[x] = x;

	//heng
	for(x=0;x<9;x++)
	{
		ret = table[py][x];
		if( (ret >= 1) && (ret <= 9) )
		{
			p[ret] = 0;
		}
	}

	//shu
	for(y=0;y<9;y++)
	{
		ret = table[y][px];
		if( (ret >= 1) && (ret <= 9) )
		{
			p[ret] = 0;
		}
	}

	//quan
	basex = px - (px%3);
	basey = py - (py%3);
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			ret = table[basey+y][basex+x];
			if( (ret >= 1) && (ret <= 9) )
			{
				p[ret] = 0;
			}
		}
	}

//say("%d,%d:	",px,py);
	//sort
	ret=0;
	for(x=1;x<10;x++)
	{
		if(p[x] == 0)continue;

		p[ret] = p[x];
//say("%d ",p[ret]);

		ret++;
	}
//say("\n");

	p[ret] = 0;
	return ret;
}
void sudoku_solve(char (*table)[9], char* buffer)
{
	int x,y,t,ret;
	int mode,count,timeout;

/*
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			ret = sudoku_possiable(x,y);
			table[y][x] = sudoku_random(possiable);
		}
	}
*/
	t = 0;		//第几个了
	mode = 0;	//进入还是回头
	timeout = 0;
	while(timeout < 9999)
	{
		y = t/9;
		x = t%9;

		//这是头一次进
		if(mode == 0)
		{
			count = sudoku_possiable(table, buffer + 10*t, x, y);
		}

		//这一次是回头
		else
		{
			for(count=0;count<10;count++)
			{
				if(buffer[10*t + count] == 0)break;
			}
		}

		//只能回头
		if(count == 0)
		{
			if(t == 0)break;	//失败退出

			table[y][x] = 0;
			t--;
			mode = 1;
		}

		//继续回头
		else if( (count == 1) && (mode != 0) )
		{
			if(t == 0)break;	//失败退出

			table[y][x] = 0;
			t--;
			mode = 1;
		}

		//选一个
		else
		{
			ret = sudoku_random(buffer+10*t, count);
			table[y][x] = ret;

			t++;
			mode = 0;

			if(t >= 81)break;	//成功退出
		}

		timeout++;
	}
}