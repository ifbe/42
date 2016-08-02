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

	//
	rectbody(
		(16*a.x) + ((16*a.y)<<16),
		(16*a.x + 15) + ((16*a.y + 15)<<16),
		0xf
	);


	//
	j=0;
	while(1)
	{
		rect(
			(16*snake[j].x) + ((16*snake[j].y)<<16),
			(16*snake[j].x + 15) + ((16*snake[j].y + 15)<<16),
			0xffffffff,
			0
		);

		j++;
		if(j>len)break;
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
		(snake[0].x > (width/16)) |
		(snake[0].y < 0) |
		(snake[0].y > (height/16)) )
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

		foodx=getrandom() % (width/16);
		foody=getrandom() % (height/16);
	}
}




static void snake_list()
{
}
static void snake_choose()
{
	//init food and snake 
	int i;
	snake[0].x=1 + getrandom() % ((width/16)-2);
	snake[0].y=1 + getrandom() % ((height/16)-2);
	len=1;

	a.x=0;
	b.x=0;

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

	snake=(struct hehe*)(addr+0x300000);
}
void snake_kill()
{
}
