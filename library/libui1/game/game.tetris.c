#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
//
void background_start(DWORD size,void* addr);
void shape_start(DWORD size,void* addr);
void ascii_start(DWORD size,void* addr);
void unicode_start(DWORD size,void* addr);
//
void line(QWORD leftup,QWORD rightdown,DWORD color);
void rect(QWORD leftup,QWORD rightdown,DWORD bodycolor,DWORD framecolor);
void printdecimal(int data,int xyz,DWORD fg,DWORD bg);
void backgroundcolor(DWORD);
//
unsigned int getrandom();
void printmemory(char*,int);
void say(char*,...);




typedef struct stucture
{
	int x;
	int y;
	int type;
	int direction;
	int x1;
	int y1;
	int x2;
	int y2;
	int x3;
	int y3;
	int x4;
	int y4;
}structure;
static structure that;

static int score=0;
static unsigned int* table;

static int width=0;
static int height=0;
static DWORD* screen=0;




static void cubie(int x,int y,int z)
{
	int haha;
	int startx,starty,endx,endy;
	DWORD bodycolor;

	haha=(height*32)/40;

	if(width<haha)
	{
		startx = x*width/32;
		endx = (x+1)*width/32 -1;
	}
	else
	{
		startx = (width-haha)/2 + (x*haha/32);
		endx = (width-haha)/2 + ((x+1)*haha/32) -1;
	}

	starty = y*height/40;
	endy = (y+1)*height/40 - 1;

	bodycolor=z>0?0xffffffff:0;

	rect(startx+(starty<<16), endx+(endy<<16), bodycolor, 0x44444444);
/*
	for(i=dx*16;i<(dx+1)*16;i++){
		for(j=y*16;j<(y+1)*16;j++){
			screen[j*width+i]=color;
		}
	}

	for(i=dx*16;i<dx*16+16;i++){
		screen[i+width*(y*16)]=0x44444444;
		screen[i+width*(y*16+16)]=0x44444444;
	}
	for(j=y*16;j<y*16+16;j++){
		screen[width*j+dx*16]=0x44444444;
		screen[width*j+dx*16+16]=0x44444444;
	}
*/
}


void generate()
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


int check()
{
	//没和已有的重合    -.    下移一格
	int check=0;
	unsigned int temp;

	if(that.x1<0 | that.x2<0 | that.x3<0 | that.x4<0) check=1;
	if(that.x1>31 | that.x2>31 | that.x3>31 | that.x4>31) check=1;

	temp=(table[that.y1]) & ( (unsigned int)1 << that.x1);
	if(temp != 0)	check=1;
	temp=(table[that.y2]) & ( (unsigned int)1 << that.x2);
	if(temp != 0)	check=1;
	temp=(table[that.y3]) & ( (unsigned int)1 << that.x3);
	if(temp != 0)	check=1;
	temp=(table[that.y4]) & ( (unsigned int)1 << that.x4);
	if(temp != 0)	check=1;

	return check;
}


void time()
{
	that.y ++;
	that.y1 ++;
	that.y2 ++;
	that.y3 ++;
	that.y4 ++;
	if(check() == 0)return;

	that.y --;
	that.y1 --;
	that.y2 --;
	that.y3 --;
	that.y4 --;

	//老的方块融入世界
	table[that.y1] |= ( (unsigned int)1 << that.x1);
	table[that.y2] |= ( (unsigned int)1 << that.x2);
	table[that.y3] |= ( (unsigned int)1 << that.x3);
	table[that.y4] |= ( (unsigned int)1 << that.x4);

	//新的方块们
	that.x=getrandom() %27+1;
	that.y=1;
	that.type=getrandom() % 7;
	that.direction=getrandom() & 0x3;
	generate();
}
void kbd(int key)
{
	if(key=='s'|key==0x28)
	{
		time();
	}
	else if(key=='a'|key==0x25)
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
	else if(key=='d'|key==0x27)
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
	else if(key=='w' | key==' '|key==0x26)		//翻转
	{
		that.direction=(that.direction +1)%4;
		generate();
		if(check(table,&that) != 0)
		{
			that.direction=(that.direction+3)%4;
		}
		generate();
	}
}
void tetris_write(QWORD type,QWORD key)
{
	int i,j;

	if(type==0x6b636f746b636974)	//ticktock
	{
		time();
	}
	else if(type==0x64626b)
	{
		kbd(key);
	}

	//one line full ?
	for(i=39;i>0;i--)
	{
		if(table[i] == 0xffffffff)
		{
			for(j=i;j>0;j--)
			{
				table[j]=table[j-1];
			}
			table[0]=0;

			score++;
		}
	}
}




void tetris_read()
{
	int i,j;
	unsigned int temp;

	for(j=0;j<40;j++)
	{
		temp=table[j];
		for(i=0;i<32;i++)
		{
			cubie(i,j,temp & 0x1);
			temp/=2;    //temp>>1 wrong,do not know why
		}
	}
	//print cubies
	cubie(that.x1,that.y1,1);
	cubie(that.x2,that.y2,1);
	cubie(that.x3,that.y3,1);
	cubie(that.x4,that.y4,1);
	//print score
	//decimal(10,10,score);
}
void tetris_list()
{
}
void tetris_choose()
{
}





static void tetris_start(DWORD size,void* addr)
{
	//1.init
	int x,y;

	//
	width=size&0xffff;
	height=(size>>16)&0xffff;
	screen=(unsigned int*)addr;
	table=(unsigned int*)(addr+0x300000);

	//
	ascii_start(size,addr);
	unicode_start(size,addr);
	background_start(size,addr);
	shape_start(size,addr);

	//
	for(x=0;x<width*height;x++)
	{
		screen[x]=0;
	}

	for(x=0;x<40;x++) table[x]=0;
	table[40]=0xffffffff;
	that.x=getrandom() %27 +1;
	that.y=1;
	that.type=5;
	that.direction=2;
	generate();

}
static void tetris_stop()
{
}
void tetris_init(char* base,void* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;		//'window'
	this[1]=0x736972746574;		//'tetris'
	this[2]=(QWORD)tetris_start;
	this[3]=(QWORD)tetris_stop;
	this[4]=(QWORD)tetris_list;
	this[5]=(QWORD)tetris_choose;
	this[6]=(QWORD)tetris_read;
	this[7]=(QWORD)tetris_write;
}
void tetris_kill()
{
}
