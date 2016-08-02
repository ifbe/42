#define QWORD unsigned long long 
#define DWORD unsigned int
//
DWORD getrandom();
void rect(DWORD p1,DWORD p2,DWORD bodycolor,DWORD framecolor);
void rectbody(DWORD p1,DWORD p2,DWORD bodycolor);
void rectframe(DWORD p1,DWORD p2,DWORD bodycolor);
//
void background_start(DWORD size,void* addr);
void shape_start(DWORD size,void* addr);
void ascii_start(DWORD size,void* addr);
void unicode_start(DWORD size,void* addr);
//
void say(char*,...);




struct hehe{
	int x;
	int y;
};
static struct hehe* snake;
static int len;
//
static struct hehe a;
static struct hehe b;
//
static int foodx;
static int foody;
//
static int width;
static int height;
static int score=0;
static int die=0;




void snake_read()
{
	//init screen
	int j;

	if(die == 1)return;

	//shadow
	if( (a.x>=0) && (a.y>=0) )
	{
		rectbody(
			(32*a.x) + ((32*a.y)<<16),
			(32*a.x + 31) + ((32*a.y + 31)<<16),
			0xf
		);
	}

	//snake
	j=0;
	while(1)
	{
		rect(
			(32*snake[j].x) + ((32*snake[j].y)<<16),
			(32*snake[j].x + 31) + ((32*snake[j].y + 31)<<16),
			0xffffffff,
			0
		);

		j++;
		if(j>=len)break;
	}

	//food
	rect(
		(32*foodx) + ((32*foody)<<16),
		(32*foodx + 31) + ((32*foody + 31)<<16),
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
		if(key=='a'|key==0x25)
		{
			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].x--;
		}
		else if(key=='d'|key==0x27)
		{
			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].x++;
		}
		else if(key=='w'|key==0x26)
		{
			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].y--;
		}
		else if(key=='s'|key==0x28)
		{
			a.x=snake[0].x;
			a.y=snake[0].y;
			snake[0].y++;
		}
		else return;
	}
	else return;

	//撞墙
	if(	(snake[0].x < 0) |
		(snake[0].x > (width/32)) |
		(snake[0].y < 0) |
		(snake[0].y > (height/32)) )
	{
		die=1;
		return;
	}

	//蛇身往前挪动
	j=0;
	while(1)
	{
		j++;
		if(j >= len)break;

		b.x = snake[j].x;
		b.y = snake[j].y;
		snake[j].x = a.x;
		snake[j].y = a.y;
		a.x = b.x;
		a.y = b.y;

		if( (snake[j].x==snake[0].x) && (snake[j].y==snake[0].y) )
		{
			die=1;
			return;
		}
	}

	//吃到食物
	if(snake[0].x==foodx && snake[0].y==foody)
	{
		snake[j].x = a.x;
		snake[j].y = a.y;
		len++;

		foodx=getrandom() % (width/32);
		foody=getrandom() % (height/32);
	}
}




static void snake_list()
{
}
static void snake_choose()
{
	//init food and snake 
	int i;
	snake[0].x=1 + getrandom() % ((width/32)-2);
	snake[0].y=1 + getrandom() % ((height/32)-2);
	foodx=getrandom() % (width/32);
	foody=getrandom() % (height/32);

	a.x = -1;
	b.x = -1;
	len=1;
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

	snake=(struct hehe*)(addr+0x300000);
}
void snake_kill()
{
}
