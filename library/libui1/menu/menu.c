#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void f1init();
void menubg();
void hidemenu();
void die();

void rectangle(QWORD,QWORD,QWORD);
void point(int x,int y,int color);
void string(int x,int y,char* str);
QWORD whereismemory();




//硬盘信息








void menushow()
{
	rectangle((256<<16)+  0 , (512<<16)+256  , 0xfefefefe);
	rectangle((256<<16)+256 , (512<<16)+512  , 0x12121212);
	rectangle((256<<16)+512 , (512<<16)+768  , 0x66666666);
	rectangle((256<<16)+768 , (512<<16)+1024 , 0xbbbbbbbb);

	string(0x3e,16,"exit");
}
void menumessage(QWORD type,QWORD key)
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
					die();
					return;
				}
			}
		}
	}
	hidemenu();
	return;
}








void menuinit(char* addr)
{
}
