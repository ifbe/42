#define QWORD unsigned long long
void writescreen();
void waitevent(QWORD* first,QWORD* second);
void decimal(int x,int y,int z);
unsigned int getrandom();




static int table[4][4];




static void cubie(int x,int y,int z)
{
	int color;
	switch(z)
	{
		case 0:color=0x55555555;break;
		case 2:color=0xfffffff0;break;
		case 4:color=0xffffffc0;break;
		case 8:color=0x995000;break;
		case 16:color=0xc05000;break;
		case 32:color=0xb03000;break;
		case 64:color=0xff0000;break;
		case 128:color=0xffffa0;break;
		case 256:color=0xffff80;break;
		case 512:color=0xffff00;break;
		case 1024:color=0xffffb0;break;
		case 2048:color=0xffffffff;break;
		case 4096:color=0xffffffff;break;
	}
	//for(i=x*160+5;i<x*160+155;i++)
	//for(j=y*160+5;j<y*160+155;j++)
	//point(i,j,color);
	rectangle(
		( x*160+5 ) + ( (y*160+5) << 16 ),
		( x*160+155 ) + ( (y*160+155) << 16 ),
		color
	);

	if(z!=0) colordecimal(4+x*20,4+y*10,z,0);
}




static void left2048()
{
	int x,y,temp;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			temp=x;
		}
	}
}
static void right2048()
{}
static void up2048()
{}
static void down2048()
{}
static void new2048()
{
	int x,y;
	int who,temp;

	//how many blank cubie
	who=0;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(table[y][x]==0)who++;
		}
	}
	if(who==0)return;

	//where
	temp = getrandom();
	if(temp<0) temp = -temp;
	who = temp % who;

	//value
	temp = getrandom()&0x3;
	if(temp>=2)temp=4;
	else temp=2;

	//put
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			//here=0 : i "can" put here
			if(table[y][x]==0)
			{
				//who=0 : i "want" put here
				if(who!=0)who--;
				else
				{
					table[y][x]=temp;
					return;
				}
			}
		}
	}
}








static void write2048(QWORD type,QWORD key)
{
	//kbd
	if(type != 0x64626b)return;

	//left,right,up,down
	if(key == 0x25)left2048();
	else if(key == 0x27)right2048();
	else if(key == 0x26)up2048();
	else if(key == 0x28)down2048();
	else return;

	//new number?
	new2048();
}
static void read2048()
{
	int x,y;
	backgroundcolor(0x444444);

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			cubie(x,y,table[y][x]);
		}
	}
	writescreen();
}
static void into2048()
{
	//1.init
	int x,y;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			table[y][x]=0;
		}
	}

	//2或4
	new2048();
}
static void list2048(QWORD* this)
{
	this[0]=0x776f646e6977;
	this[1]=0x38343032;
	this[2]=(0<<16)+0;		//left,up
	this[3]=(640<<16)+640;		//right,down
	this[4]=(QWORD)list2048;
	this[5]=(QWORD)into2048;
	this[6]=(QWORD)read2048;
	this[7]=(QWORD)write2048;
}








void register2048(char* in)
{
	list2048( (QWORD*)in );
}
