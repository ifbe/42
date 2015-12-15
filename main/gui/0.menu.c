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
	if(type!=0x7466656C207A7978)return;	//'xyz left'

	int x=key&0xffff;
	int y=(key>>16)&0xffff;
	if(y>256)
	{
		if(y<512)
		{
			serventreport(-1);
			return;
		}
	}

	//hidemenu
	serventreport(0);
	return;
}








void menuinit(char* addr)
{
	QWORD* that=(QWORD*)addr;
	that[0]=0x776f646e6977;		//'window'
	that[1]=0x756e656d;		//'menu'
	that[2]=((QWORD)1<<32)+(256<<16)+256;		//startaddr
	that[3]=((QWORD)1<<32)+(512<<16)+768;		//endaddr
	that[4]=(QWORD)menushow;
	that[5]=(QWORD)menumessage;
}
