#define u8 unsigned char




int tetris_newfood(u8* buf, int w, int h)
{
	buf[(0*w)+7] = 0x80;
	buf[(1*w)+7] = 0x80;
	buf[(2*w)+7] = 0x80;
	buf[(3*w)+7] = 0x80;
	return 0;
}
int tetris_generate(u8* buf, int w, int h)
{
	int j;
	for(j=0;j<w*h;j++)buf[j] = 0;
	tetris_newfood(buf, w, h);
	return 0;
}
int tetris_left(u8* buf, int w, int h)
{
	int x,y,k;
	int m[4];
	int n[4];

	k = 0;
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			if(0x80 == (buf[(y*w)+x]&0x80))
			{
				m[k] = x;
				n[k] = y;
				k++;
			}
		}
	}
	if(k != 4)return 0;

	if(m[0] <= 0)return 0;
	if(m[1] <= 0)return 0;
	if(m[2] <= 0)return 0;
	if(m[3] <= 0)return 0;

	if(0 != (buf[(n[0]*w) + m[0]-1]&0x1))return 0;
	if(0 != (buf[(n[1]*w) + m[1]-1]&0x1))return 0;
	if(0 != (buf[(n[2]*w) + m[2]-1]&0x1))return 0;
	if(0 != (buf[(n[3]*w) + m[3]-1]&0x1))return 0;

	buf[(n[0]*w) + m[0]] = 0;
	buf[(n[1]*w) + m[1]] = 0;
	buf[(n[2]*w) + m[2]] = 0;
	buf[(n[3]*w) + m[3]] = 0;

	buf[(n[0]*w) + m[0]-1] = 0x80;
	buf[(n[1]*w) + m[1]-1] = 0x80;
	buf[(n[2]*w) + m[2]-1] = 0x80;
	buf[(n[3]*w) + m[3]-1] = 0x80;
	return 1;
}
int tetris_right(u8* buf, int w, int h)
{
	int x,y,k;
	int m[4];
	int n[4];

	k = 0;
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			if(0x80 == (buf[(y*w)+x]&0x80))
			{
				m[k] = x;
				n[k] = y;
				k++;
			}
		}
	}
	if(k != 4)return 0;

	if(m[0]+1 >= w)return 0;
	if(m[1]+1 >= w)return 0;
	if(m[2]+1 >= w)return 0;
	if(m[3]+1 >= w)return 0;

	if(0 != (buf[(n[0]*w) + m[0]+1]&0x1))return 0;
	if(0 != (buf[(n[1]*w) + m[1]+1]&0x1))return 0;
	if(0 != (buf[(n[2]*w) + m[2]+1]&0x1))return 0;
	if(0 != (buf[(n[3]*w) + m[3]+1]&0x1))return 0;

	buf[(n[0]*w) + m[0]] = 0;
	buf[(n[1]*w) + m[1]] = 0;
	buf[(n[2]*w) + m[2]] = 0;
	buf[(n[3]*w) + m[3]] = 0;

	buf[(n[0]*w) + m[0]+1] = 0x80;
	buf[(n[1]*w) + m[1]+1] = 0x80;
	buf[(n[2]*w) + m[2]+1] = 0x80;
	buf[(n[3]*w) + m[3]+1] = 0x80;
	return 1;
}
int tetris_up(u8* buf, int w, int h)
{
	return 0;
}
int tetris_down(u8* buf, int w, int h)
{
	int x,y,k;
	int m[4];
	int n[4];

	k = 0;
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			if(0x80 == (buf[(y*w)+x]&0x80))
			{
				m[k] = x;
				n[k] = y;
				k++;
			}
		}
	}
	if(k != 4)return 0;

	if(n[0]+1 >= h)goto merged;
	if(n[1]+1 >= h)goto merged;
	if(n[2]+1 >= h)goto merged;
	if(n[3]+1 >= h)goto merged;

	if(0 != (buf[w*(n[0]+1) + m[0]]&0x1))goto merged;
	if(0 != (buf[w*(n[1]+1) + m[1]]&0x1))goto merged;
	if(0 != (buf[w*(n[2]+1) + m[2]]&0x1))goto merged;
	if(0 != (buf[w*(n[3]+1) + m[3]]&0x1))goto merged;

	buf[w*n[0] + m[0]] = 0;
	buf[w*n[1] + m[1]] = 0;
	buf[w*n[2] + m[2]] = 0;
	buf[w*n[3] + m[3]] = 0;

	buf[w*(n[0]+1) + m[0]] = 0x80;
	buf[w*(n[1]+1) + m[1]] = 0x80;
	buf[w*(n[2]+1) + m[2]] = 0x80;
	buf[w*(n[3]+1) + m[3]] = 0x80;
	return 1;

merged:
	buf[w*n[0] + m[0]] = 1;
	buf[w*n[1] + m[1]] = 1;
	buf[w*n[2] + m[2]] = 1;
	buf[w*n[3] + m[3]] = 1;

	tetris_newfood(buf, w, h);
	return 1;
}
/*
int tetris_generate()
{
	if(that.type==0)
	{
		//	0000
		if(that.direction==1 | that.direction==3)
		{
			that.x1=that.x;
			that.x2=that.x +1;
			that.x3=that.x +2;
			that.x4=that.x +3;
			that.y1=that.y2=that.y3=that.y4=that.y;
		}

		//	0
		//	0
		//	0
		//	0
		if(that.direction==0 | that.direction==2)
		{
			that.x1=that.x2=that.x3=that.x4=that.x;
			that.y1=that.y;
			that.y2=that.y +1;
			that.y3=that.y +2;
			that.y4=that.y +3;
		}
	}
	if(that.type==1)
	{
		//	000
		//	 0
		if(that.direction==0)
		{	that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x;
			that.y4=that.y +1;
		}

		//	0
		//	00
		//	0
		if(that.direction==1)
		{	that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y;
		}

		//	 0
		//	000
		if(that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x;
			that.y4=that.y -1;
		}

		//	 0
		//	00
		//	 0
		if(that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x;
			that.y4=that.y -1;
		}
	}
	if(that.type==2)
	{
		//	0
		//	00
		//	 0
		if(that.direction==0|that.direction==2)
		{	that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x -1;
			that.y3=that.y -1;
			that.x4=that.x;
			that.y4=that.y+1;
		}

		//	 00
		//	00
		if(that.direction==1|that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x +1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x -1;
			that.y4=that.y +1;
		}
	}
	if(that.type==3)
	{
		//	 0
		//	00
		//	0
		if(that.direction==0|that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x +1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y -1;
		}

		//	00
		//	 00
		if(that.direction==1|that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x +1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y -1;
			that.x4=that.x -1;
			that.y4=that.y -1;
		}
	}
	if(that.type==4)
	{
		//	00
		//	0
		//	0
		if(that.direction==0)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y -1;
		}

		//	0
		//	000
		if(that.direction==1)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x -1;
			that.y4=that.y -1;
		}

		//	 0
		//	 0
		//	00
		if(that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x -1;
			that.y4=that.y +1;
		}

		//	000
		//	  0
		if(that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x +1;
			that.y4=that.y +1;
		}
	}
	if(that.type==5)
	{
		//	00
		//	 0
		//	 0
		if(that.direction==0)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x -1;
			that.y4=that.y -1;
		}

		//	000
		//	0
		if(that.direction==1)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x -1;
			that.y4=that.y +1;
		}

		//	0
		//	0
		//	00
		if(that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y +1;
		}

		//	  0
		//	000
		if(that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x +1;
			that.y4=that.y -1;
		}
	}

	//	00
	//	00
	if(that.type==6)
	{
		that.x1=that.x;
		that.y1=that.y;
		that.x2=that.x +1;
		that.y2=that.y;
		that.x3=that.x;
		that.y3=that.y +1;
		that.x4=that.x +1;
		that.y4=that.y +1;
	}
}




static int tetris_check()
{
	unsigned int temp;

	//left
	if(that.x1<0)return 1;
	if(that.x2<0)return 2;
	if(that.x3<0)return 3;
	if(that.x4<0)return 4;

	//right
	if(that.x1>31)return 5;
	if(that.x2>31)return 6;
	if(that.x3>31)return 7;
	if(that.x4>31)return 8;

	//collision
	if(table[32 * that.y1 + that.x1] != 0)return 9;
	if(table[32 * that.y2 + that.x2] != 0)return 10;
	if(table[32 * that.y3 + that.x3] != 0)return 11;
	if(table[32 * that.y4 + that.x4] != 0)return 12;

	//success
	return 0;
}


static int tetris_left()
{
	if(that.x1>0&&that.x2>0&&that.x3>0&&that.x4>0)
	{
		that.x --;
		that.x1 --;
		that.x2 --;
		that.x3 --;
		that.x4 --;
		if(check() != 0)
		{
			that.x ++;
			that.x1 ++;
			that.x2 ++;
			that.x3 ++;
			that.x4 ++;
		}
	}
}
static int tetris_right()
{
	if(that.x1<31&&that.x2<31&&that.x3<31&&that.x4<31)
	{
		that.x ++;
		that.x1 ++;
		that.x2 ++;
		that.x3 ++;
		that.x4 ++;
		if(check() != 0)
		{
			that.x --;
			that.x1 --;
			that.x2 --;
			that.x3 --;
			that.x4 --;
		}
	}
}
static int tetris_up()
{
	that.direction=(that.direction +1)%4;
	generate();
	if(check() != 0)
	{
		that.direction=(that.direction+3)%4;
	}
	generate();
}
static int tetris_down()
{
	int x,y,count;

	that.y ++;
	that.y1 ++;
	that.y2 ++;
	that.y3 ++;
	that.y4 ++;
	if(check() < 9)return 0;

	that.y --;
	that.y1 --;
	that.y2 --;
	that.y3 --;
	that.y4 --;

	table[32 * that.y1 + that.x1]=1;
	table[32 * that.y2 + that.x2]=1;
	table[32 * that.y3 + that.x3]=1;
	table[32 * that.y4 + that.x4]=1;

	y=39;
	while(1)
	{
		count=0;
		for(x=0;x<32;x++)
		{
			if(table[32*y+x]>0)count++;
		}

		if(count==32)
		{
			//
			for(x=y*32+31;x>32;x--)
			{
				table[x]=table[x-32];
			}
		}

		else
		{
			y--;
			if(y==0)break;
		}
	}

	//
	that.x=getrandom() %27+1;
	that.y=1;
	that.type=getrandom() % 7;
	that.direction=getrandom() & 0x3;
	generate();
	return 1;
}
*/
