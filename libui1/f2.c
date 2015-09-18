#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




void background2();
void point(int x,int y,DWORD color);
void string(int x,int y,char* str);
void hexadecimal(int x,int y,QWORD in);
void mount(QWORD in);
QWORD whereisworld();




//static QWORD partbuffer;
static QWORD dirbuffer;
static char choosepart=0;
static char choosefile=0;





/*
void kuangjia()
{
	int x,y;
	for(y=0;y<32;y++)
	{
		point(0x100,y,0xff00);
		point(0x200,y,0xff00);
		point(0x300,y,0xff00);
	}
	for(x=0;x<1024;x++)
	{
		point(x,0x20,0xff00);
		point(x,639-32,0xff00);
	}
}*/
void printfile0()
{
	int x,y;
	char* p;

//三.每个分区里面的文件和文件夹
	p=(char*)dirbuffer;
	string(0x10,1,"name");
	string(0x30,1,"id");
	string(0x50,1,"type");
	string(0x70,1,"size");
	for(y=0;y<36;y++)
	{
		if(*(DWORD*)(p+0x40*y) == 0) break;
		string(0,y+2,p+0x40*y);
		hexadecimal(0x30,y+2,*(QWORD*)(p+0x40*y+0x10));
		hexadecimal(0x50,y+2,*(QWORD*)(p+0x40*y+0x20));
		hexadecimal(0x70,y+2,*(QWORD*)(p+0x40*y+0x30));
	}
/*
	//选中
	for(y=32+choosefile*16;y<48+choosefile*16;y+=2)
	{
		for(x=0;x<1024;x+=2)
		{
			point(x,y,0);
		}
	}
*/
}








void f2show()
{
	background2();
	//kuangjia();
	printfile0();
}
void f2message(QWORD type,QWORD key)
{
	//if(type!=2)return;
	if(type!=0x7466656c)return;

	int x=key&0xffff;
	int y=(key>>16)&0xffff;
	if(y>639-32)		//选分区
	{
		choosepart=x/128;
		mount(choosepart);
	}
	else if(y>32)		//选文件
	{
		choosefile=(y-32)/16;
	}
}








void f2init(QWORD world)
{
	dirbuffer=world+0x200000;
}