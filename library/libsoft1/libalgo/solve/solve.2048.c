#define u8 unsigned char
unsigned int random_read();




void left2048(u8 (*table)[4])
{
	int x,y;
	int dst,temp;

	for(y=0;y<4;y++)
	{
		for(x=0;x<3;x++)
		{
			if(table[y][x] != 0)
			{
				dst = x+1;
				for(;dst<4;dst++)
				{
					if(table[y][dst] == 0)continue;

					if(table[y][dst] != table[y][x])break;

					table[y][x]++;
					table[y][dst] = 0;
					x = dst;
					break;
				}
			}//if
		}//forx

		dst=0;
		for(x=0;x<4;x++)
		{
			if(table[y][x] != 0)
			{
				temp = table[y][x];
				table[y][x] = 0;
				table[y][dst] = temp;
				dst++;
			}
		}
	}//fory
}
void right2048(u8 (*table)[4])
{
	int x,y;
	int dst,temp;

	for(y=0;y<4;y++)
	{
		for(x=3;x>0;x--)
		{
			if(table[y][x] != 0)
			{
				dst = x-1;
				for(;dst>=0;dst--)
				{
					if(table[y][dst] == 0 )continue;
					if(table[y][dst] != table[y][x] )break;
					table[y][x]++;
					table[y][dst] = 0;
					x = dst;
					break;
				}
			}//if
		}//forx

		dst=3;
		for(x=3;x>=0;x--)
		{
			if(table[y][x] != 0)
			{
				temp = table[y][x];
				table[y][x] = 0;
				table[y][dst] = temp;
				dst--;
			}
		}
	}//fory
}
void up2048(u8 (*table)[4])
{
	int x,y;
	int dst,temp;

	for(x=0;x<4;x++)
	{
		for(y=0;y<3;y++)
		{
			if(table[y][x] != 0)
			{
				dst = y+1;
				for(;dst<4;dst++)
				{
					if(table[dst][x] == 0 )continue;
					if(table[dst][x] != table[y][x] )break;
					table[y][x]++;
					table[dst][x] = 0;
					y = dst;
					break;
				}
			}//if
		}//fory

		dst=0;
		for(y=0;y<4;y++)
		{
			if(table[y][x] != 0)
			{
				temp = table[y][x];
				table[y][x] = 0;
				table[dst][x] = temp;
				dst++;
			}
		}
	}//forx
}
void down2048(u8 (*table)[4])
{
	int x,y;
	int dst,temp;

	for(x=0;x<4;x++)
	{
		for(y=3;y>0;y--)
		{
			if(table[y][x] != 0)
			{
				dst = y-1;
				for(;dst>=0;dst--)
				{
					if(table[dst][x] == 0 )continue;
					if(table[dst][x] != table[y][x] )break;
					table[y][x]++;
					table[dst][x] = 0;
					y = dst;
					break;
				}
			}//if
		}//forx

		dst=3;
		for(y=3;y>=0;y--)
		{
			if(table[y][x]!=0)
			{
				temp = table[y][x];
				table[y][x] = 0;
				table[dst][x] = temp;
				dst--;
			}
		}
	}//fory
}
void new2048(u8 (*table)[4])
{
	int x,y,temp,count;

	//how many blank cubie
	count=0;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(table[y][x]==0)count++;
		}
	}
	if(count==0)return;
	//say("count=%x\n", count);

	//choose which empty cubie
	count = random_read() % count;
	//say("choice=%x\n", count);
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(table[y][x] == 0)
			{
				if(count == 0)
				{
					table[y][x] = (random_read()&0x1) + 1;
					return;
				}
				else count--;
			}//if(empty)
		}//for(x)
	}//for(y)
}