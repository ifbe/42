#define QWORD unsigned long long 
#define DWORD unsigned int
//
DWORD getrandom();
void rect(DWORD p1,DWORD p2,DWORD bodycolor,DWORD framecolor);
//
void background_start(DWORD size,void* addr);
void shape_start(DWORD size,void* addr);
void ascii_start(DWORD size,void* addr);
void unicode_start(DWORD size,void* addr);
//
void say(char*,...);




struct snake{
	int x;
	int y;
	int direction;
};
static struct snake* snake;
static int length;
//
static int foodx;
static int foody;
//
static int score=0;
static int die=0;
//
static int width;
static int height;




void snake_read()
{
	//init screen
	int j;
	if(die == 1)return;

	//
	rect(
		(16*snake[length-1].x - 16) + ((16*snake[length-1].y - 16)<<16),
		(16*snake[length-1].x + 31) + ((16*snake[length-1].y + 31)<<16),
		0,
		0
	);


	//
	j=0;
	while( snake[j].direction != 0 )
	{
say("%d:%d,%d,%d\n",j,snake[j].x,snake[j].y,snake[j].direction);
		rect(
			(16*snake[j].x) + ((16*snake[j].y)<<16),
			(16*snake[j].x + 15) + ((16*snake[j].y + 15)<<16),
			0xffffffff,
			0
		);
		j++;
	}

	//
	rect(
		(16*foodx) + ((16*foody)<<16),
		(16*foodx + 15) + ((16*foody + 15)<<16),
		0xff00,
		0
	);
}




void snake_write(QWORD type,QWORD key)
{
	int j;
	if(die==1)return;

	if(type==0x64626b)
	{
		snake[length].x=snake[length-1].x;
		snake[length].y=snake[length-1].y;
		snake[length].direction=snake[length-1].direction;
		snake[1].direction=snake[0].direction;

		if(key=='a'|key==0x25)
		{
			snake[0].direction=1;
			snake[0].x--;
		}
		else if(key=='d'|key==0x27)
		{
			snake[0].direction=2;
			snake[0].x++;
		}
		else if(key=='w'|key==0x26)
		{
			snake[0].direction=3;
			snake[0].y--;
		}
		else if(key=='s'|key==0x28)
		{
			snake[0].direction=4;
			snake[0].y++;
		}
	}
	else return;
/*
	else
	{
		switch(snake[0].direction)
		{
			case 1:snake[0].x--; break;
			case 2:snake[0].x++; break;
			case 3:snake[0].y--; break;
			case 4:snake[0].y++; break;
		}
	}
*/
	//撞墙
	if(	(snake[0].x < 0) |
		(snake[0].x > (width/16)) |
		(snake[0].y < 0) |
		(snake[0].y > (height/16)) )
	{
		die=1;
		return;
	}

	//吃到食物
	if(snake[0].x==foodx && snake[0].y==foody)
	{
		foodx=getrandom() % (width/16);
		foody=getrandom() % (height/16);

		length++;
	}

	//蛇身往前挪动
	j=1;
	while(1)
	{
		if(j >= length)
		{
			snake[j].direction = 0;
			break;
		}

		if(snake[j].direction == 1)
		{
			snake[j].x--;
		}
		else if(snake[j].direction == 2)
		{
			snake[j].x++;
		}
		else if(snake[j].direction == 3)
		{
			snake[j].y--;
		}
		else if(snake[j].direction == 4)
		{
			snake[j].y++;
		}
/*
		if(snake[0].x==snake[j].x && snake[0].y==snake[j].y)
		{
			die=1;
		}
*/
		snake[j].direction = snake[j-1].direction;
		j++;
	}

}




static void snake_list()
{
}
static void snake_choose()
{
	//init food and snake 
	int i;
	for(i=1;i<100;i++) snake[i].direction=0;
	snake[0].x=1 + getrandom() % ((width/16)-2);
	snake[0].y=1 + getrandom() % ((height/16)-2);
	snake[0].direction=1;
	snake[1].x=snake[0].x+1;
	snake[1].y=snake[0].y;
	snake[1].direction=1;
	length=2;

	foodx=getrandom() % (width/16);
	foody=getrandom() % (height/16);
	die=0;
}




static void snake_start(DWORD size,void* addr)
{
	//1.init
	int x;
	int* p;

	//
	width=size&0xffff;
	height=(size>>16)&0xffff;
	p=(int*)addr;

	//
	ascii_start(size,addr);
	unicode_start(size,addr);
	background_start(size,addr);
	shape_start(size,addr);

	//
	for(x=0;x<width*height;x++)
	{
		p[x]=0;
	}
}
static void snake_stop()
{
}




void snake_init(char* base,void* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;		//'window'
	this[1]=0x656b616e73;		//'snake'
	this[2]=(QWORD)snake_start;
	this[3]=(QWORD)snake_stop;
	this[4]=(QWORD)snake_list;
	this[5]=(QWORD)snake_choose;
	this[6]=(QWORD)snake_read;
	this[7]=(QWORD)snake_write;

	snake=(struct snake*)(addr+0x300000);
}
void snake_kill()
{
}
