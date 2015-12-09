#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void f1init();
void menubg();

void rectangle(QWORD,QWORD,QWORD);
void point(int x,int y,int color);
void string(int x,int y,char* str);

void serventreport(int);
QWORD whereismemory();








static void menushow()
{
	rectangle((256<<16)+  0 , (512<<16)+256  , 0x44);
	rectangle((256<<16)+256 , (512<<16)+512  , 0x4400);
	rectangle((256<<16)+512 , (512<<16)+768  , 0x440000);
	rectangle((256<<16)+768 , (512<<16)+1024 , 0x4080c0);

	string(0x3e,16,"exit");
}
static void menumessage(QWORD type,QWORD key)
{
	if(type!=0x7466656c)return;		//不是鼠标

	int x=key&0xffff;
	int y=(key>>16)&0xffff;
	if(y>256)
	{
		if(y<256+64)
		{
			if(x>512-64)
			{
				if(x<512+64)
				{
					serventreport(-1);
					return;
				}
			}
		}
	}
	//hidemenu
	return;
}








void menuinit(char* addr)
{
	QWORD* that=(QWORD*)addr;
	that[0]=0x756e656d;	//'menu'
	that[8]=(QWORD)menushow;
	that[9]=(QWORD)menumessage;
}
