#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
u32 getrandom();
struct snake
{
	u8 x;
	u8 y;
	u16 next;
};
//0:	footx, foody, freespace
//n:	snakex, snakey, nextnode




int snake_newfood(struct snake* buf, int w, int h)
{
	buf[0].x = getrandom() % w;
	buf[0].y = getrandom() % h;
}
int snake_generate(struct snake* buf, int w, int h)
{
	int j;
	buf[0].next = 2;

	buf[1].x = 9;
	buf[1].y = 9;
	buf[1].next = 0;

	for(j=2;j<w*h;j++)
	{
		buf[j].x = 0;
		buf[j].y = 0;
		buf[j].next = 0;
	}

	snake_newfood(buf, w, h);
}
int snake_left(struct snake* buf, int w, int h)
{
	int x,y,m,n;
	if(buf[1].x <= 0)return 0;

	n = buf[0].next;
	x = buf[1].x;
	y = buf[1].y;
	if((x == buf[0].x+1) && (y == buf[0].y))
	{
		buf[n].x = buf[1].x;
		buf[n].y = buf[1].y;
		buf[n].next = buf[1].next;

		buf[1].x = buf[0].x;
		buf[1].y = buf[0].y;
		buf[1].next = n;

		buf[0].next += 1;
		snake_newfood(buf, w, h);
		return 1;
	}

	m = n = 1;
	while(1)
	{
		if(buf[n].next <= 1)
		{
			buf[m].next = 0;
			break;
		}

		m = n;
		n = buf[n].next;
	}
	if(1 == n)
	{
		buf[1].x -= 1;
		return 1;
	}

	x = buf[1].x;
	y = buf[1].y;
	m = buf[1].next;

	buf[1].x -= 1;
	//buf[1].y
	buf[1].next = n;

	buf[n].x = x;
	buf[n].y = y;
	if(m != n)buf[n].next = m;
}
int snake_right(struct snake* buf, int w, int h)
{
	int x,y,m,n;
	if(buf[1].x+1 >= w)return 0;

	n = buf[0].next;
	x = buf[1].x;
	y = buf[1].y;

	if((x+1 == buf[0].x) && (y == buf[0].y))
	{
		buf[n].x = buf[1].x;
		buf[n].y = buf[1].y;
		buf[n].next = buf[1].next;

		buf[1].x = buf[0].x;
		buf[1].y = buf[0].y;
		buf[1].next = n;

		buf[0].next += 1;
		snake_newfood(buf, w, h);
		return 1;
	}

	m = n = 1;
	while(1)
	{
		if(buf[n].next <= 1)
		{
			buf[m].next = 0;
			break;
		}

		m = n;
		n = buf[n].next;
	}
	if(1 == n)
	{
		buf[1].x += 1;
		return 1;
	}

	x = buf[1].x;
	y = buf[1].y;
	m = buf[1].next;

	buf[1].x += 1;
	//buf[1].y
	buf[1].next = n;

	buf[n].x = x;
	buf[n].y = y;
	if(m != n)buf[n].next = m;
}
int snake_up(struct snake* buf, int w, int h)
{
	int x,y,m,n;
	if(buf[1].y <= 0)return 0;

	n = buf[0].next;
	x = buf[1].x;
	y = buf[1].y;

	if((x == buf[0].x) && (y == buf[0].y+1))
	{
		buf[n].x = buf[1].x;
		buf[n].y = buf[1].y;
		buf[n].next = buf[1].next;

		buf[1].x = buf[0].x;
		buf[1].y = buf[0].y;
		buf[1].next = n;

		buf[0].next += 1;
		snake_newfood(buf, w, h);
		return 1;
	}

	m = n = 1;
	while(1)
	{
		if(buf[n].next <= 1)
		{
			buf[m].next = 0;
			break;
		}

		m = n;
		n = buf[n].next;
	}
	if(1 == n)
	{
		buf[1].y -= 1;
		return 1;
	}

	x = buf[1].x;
	y = buf[1].y;
	m = buf[1].next;

	//buf[1].x
	buf[1].y -= 1;
	buf[1].next = n;

	buf[n].x = x;
	buf[n].y = y;
	if(m != n)buf[n].next = m;
}
int snake_down(struct snake* buf, int w, int h)
{
	int x,y,m,n;
	if(buf[1].y+1 >= h)return 0;

	n = buf[0].next;
	x = buf[1].x;
	y = buf[1].y;

	if((x == buf[0].x) && (y+1 == buf[0].y))
	{
		buf[n].x = buf[1].x;
		buf[n].y = buf[1].y;
		buf[n].next = buf[1].next;

		buf[1].x = buf[0].x;
		buf[1].y = buf[0].y;
		buf[1].next = n;

		buf[0].next += 1;
		snake_newfood(buf, w, h);
		return 1;
	}

	m = n = 1;
	while(1)
	{
		if(buf[n].next <= 1)
		{
			buf[m].next = 0;
			break;
		}

		m = n;
		n = buf[n].next;
	}
	if(1 == n)
	{
		buf[1].y += 1;
		return 1;
	}

	x = buf[1].x;
	y = buf[1].y;
	m = buf[1].next;

	//buf[1].x
	buf[1].y += 1;
	buf[1].next = n;

	buf[n].x = x;
	buf[n].y = y;
	if(m != n)buf[n].next = m;
}