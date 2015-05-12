#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

static QWORD dirbuffer;

static int complex=0;		//主体华丽程度




void initlogic0(QWORD in)
{
	dirbuffer=in;
}
void printfile0()
{
	int x,y;
	char* p;

//三.每个分区里面的文件和文件夹
	p=(char*)dirbuffer;
	string(0,0,"name");
	string(16,0,"id");
	string(32,0,"type");
	string(48,0,"size");
	for(y=0;y<36;y++)
	{
		if(*(DWORD*)(p+0x40*y) == 0) break;
		string(0,y+1,p+0x40*y);
		hexadecimal(16,y+1,*(QWORD*)(p+0x40*y+0x10));
		hexadecimal(32,y+1,*(QWORD*)(p+0x40*y+0x20));
		hexadecimal(48,y+1,*(QWORD*)(p+0x40*y+0x30));
	}
}
void printfile1()
{
	
}
void printfile2()
{
	
}
void printfile()
{
	if(complex==0)
	{
		printfile0();
	}
	else if(complex==1)
	{
		printfile1();
	}
	else
	{
		printfile2();
	}
}
mouseinputforfile()
{
	
}
kbdinputforfile()
{
	
}