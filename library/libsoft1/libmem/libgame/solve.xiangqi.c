void xiangqi_move(char (*data)[9], int* turn, int px, int py, int x, int y)
{
	int min, max, t, u;
	say("(%d,%d) -> (%d,%d)\n", px, py, x, y);

	//chess going
	if(data[py][px] == 'S')		//兵
	{
		if(
			( (x == px) && (y == py+1) ) |
			( (y>4) && (x == px-1) && (y == py) ) |
			( (y>4) && (x == px+1) && (y == py) )
		)
		{
			if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				goto success;
			}
		}
	}
	else if(data[py][px] == 's')	//兵
	{
		if(
			( (x == px) && (y == py-1) ) |
			( (y<5) && (x == px-1) && (y == py) ) |
			( (y<5) && (x == px+1) && (y == py) )
		)
		{
			if( (data[y][x] < 'a') | (data[y][x] > 'z') )
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				goto success;
			}
		}
	}
	else if(data[py][px] == 'Z')	//炮
	{
		if(data[y][x] == 0)
		{
			//横移
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if( data[y][t] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}

			//竖移
			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}

		//打隔子
		else if(data[y][x] >= 'a')
		{
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[y][t] != 0)u++;
				}
				if(u == 1)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}

			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 1)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}
	}
	else if(data[py][px] == 'z')	//炮
	{
		if(data[y][x] == 0)
		{
			//横移
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if( data[y][t] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}

			//竖移
			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}

		//打隔子
		else if( (data[y][x] >= 'A') && (data[y][x] <= 'Z') )
		{
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[y][t] != 0)u++;
				}
				if(u == 1)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}

			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 1)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}
	}
	else if(data[py][px] == 'A')	//车
	{
		if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
		{
			//横移
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if( data[y][t] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}

			//竖移
			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}
	}
	else if(data[py][px] == 'a')	//车
	{
		if( (data[y][x] < 'a') | (data[y][x] > 'z') )
		{
			//横移
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if( data[y][t] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}

			//竖移
			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}
	}
	else if(data[py][px] == 'B')	//马
	{
		if(data[y][x] < 'A' | data[y][x] > 'Z')
		{
			if( (x == px-2) && (y == py-1) | (y == py+1) )
			{
				if(data[py][px-1] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
			else if( (x == px+2) && (y == py-1) | (y == py+1) )
			{
				if(data[py][px+1] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
			else if( (y == py-2) && ( (x == px-1) | (x == px+1) ) )
			{
				if(data[py-1][px] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
			else if( (y == py+2) && ( (x == px-1) | (x == px+1) ) )
			{
				if(data[py+1][px] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}
	}
	else if(data[py][px] == 'b')	//马
	{
		if(data[y][x] < 'a' | data[y][x] > 'z')
		{
			if( (x == px-2) && (y == py-1) | (y == py+1) )
			{
				if(data[py][px-1] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
			else if( (x == px+2) && (y == py-1) | (y == py+1) )
			{
				if(data[py][px+1] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
			else if( (y == py-2) && ( (x == px-1) | (x == px+1) ) )
			{
				if(data[py-1][px] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
			else if( (y == py+2) && ( (x == px-1) | (x == px+1) ) )
			{
				if(data[py+1][px] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}
	}
	else if(data[py][px] == 'C')	//象
	{
		if( (y<5) && ( (data[y][x] < 'A') | (data[y][x] > 'Z') ) )
		{
			if(
			( (x == px-2) && (y == py-2) && (data[py-1][px-1]==0) )|
			( (x == px-2) && (y == py+2) && (data[py+1][px-1]==0) )|
			( (x == px+2) && (y == py-2) && (data[py-1][px+1]==0) )|
			( (x == px+2) && (y == py+2) && (data[py+1][px+1]==0) )
			)
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				goto success;
			}
		}
	}
	else if(data[py][px] == 'c')	//象
	{
		if( (y>4) && ( (data[y][x] < 'a') | (data[y][x] > 'z') ) )
		{
			if(
			( (x == px-2) && (y == py-2) && (data[py-1][px-1]==0) )|
			( (x == px-2) && (y == py+2) && (data[py+1][px-1]==0) )|
			( (x == px+2) && (y == py-2) && (data[py-1][px+1]==0) )|
			( (x == px+2) && (y == py+2) && (data[py+1][px+1]==0) )
			)
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				goto success;
			}
		}
	}
	else if(data[py][px] == 'D')	//士
	{
		if( (x>=3)&&(x<=5)&&(y>=0)&&(y<=2) )
		{
			if(
				( (x == px-1) && (y == py-1) ) |
				( (x == px-1) && (y == py+1) ) |
				( (x == px+1) && (y == py-1) ) |
				( (x == px+1) && (y == py+1) )
			)
			{
				if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}
	}
	else if(data[py][px] == 'd')	//士
	{
		if( (x>=3)&&(x<=5)&&(y>=7)&&(y<=9) )
		{
			if(
				( (x == px+1) && (y == py-1) ) |
				( (x == px+1) && (y == py+1) ) |
				( (x == px-1) && (y == py-1) ) |
				( (x == px-1) && (y == py+1) )
			)
			{
				if( (data[y][x] < 'a') | (data[y][x] > 'z') )
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}
	}
	else if(data[py][px] == 'E')	//将
	{
		if( (x>=3)&&(x<=5)&&(y>=0)&&(y<=2) )
		{
			if(
				( (x == px) && (y == py-1) ) |
				( (x == px) && (y == py+1) ) |
				( (x == px-1) && (y == py) ) |
				( (x == px+1) && (y == py) )
			)
			{
				if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}

		else if( (x == px) && (data[y][x] == 'e') )
		{
			u=0;
			for(t=py+1;t<y;t++)
			{
				if(data[t][px] != 0)u++;
			}
			if(u == 0)
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				goto success;
			}
		}
	}
	else if(data[py][px] == 'e')	//将
	{
		if( (x>=3)&&(x<=5)&&(y>=7)&&(y<=9) )
		{
			if(
				( (x == px) && (y == py-1) ) |
				( (x == px) && (y == py+1) ) |
				( (x == px-1) && (y == py) ) |
				( (x == px+1) && (y == py) )
			)
			{
				if( (data[y][x] < 'a') | (data[y][x] > 'z') )
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					goto success;
				}
			}
		}

		else if( (x == px) && (data[y][x] == 'E') )
		{
			u=0;
			for(t=y+1;t<py;t++)
			{
				if(data[t][px] != 0)u++;
			}
			if(u == 0)
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				goto success;
			}
		}
	}

fail:
	return;

success:
	(*turn)++;
	return;
}