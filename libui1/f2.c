#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void background2();

void string(int x,int y,char* str);
void hexadecimal(int x,int y,QWORD in);
void decimal(int x,int y,QWORD in);

double calculator(char* postfix);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);

QWORD whereisworld();








//
static int count=0;

static char buffer[128];
static char postfix[128];

static double result;

//
static QWORD dirbuffer;




void printfile0()
{
	int x,y;
	char* p=(char*)dirbuffer;

	for(y=0;y<36;y++)
	{
		if(*(DWORD*)(p+0x40*y) == 0) break;
		string(0,y+2,p+0x40*y);
		hexadecimal(0x30,y+2,*(QWORD*)(p+0x40*y+0x10));
		hexadecimal(0x50,y+2,*(QWORD*)(p+0x40*y+0x20));
		hexadecimal(0x70,y+2,*(QWORD*)(p+0x40*y+0x30));
	}
}
void f2show()
{
	char table[100];

	background2();

	printfile0();

	string(0,0,buffer);
	string(0,1,postfix);
	double2decimalstring(result,table);
	string(0,2,table);
}
void f2message(QWORD type,QWORD key)
{
	if(type==0x72616863)		//char
	{
		if(key==0x8)			//backspace
		{
			if(count!=0)count--;
			buffer[count]=0;
		}
		else if(key==0xd)		//enter
		{
			//134+95*x+(70*44+f)*g ----> 134 95 x * + 70 44 * f + g * +
			infix2postfix(buffer,postfix);
			result=calculator(postfix);

			for(count=0;count<127;count++) buffer[count]=0;
			count=0;
		}
		else
		{
			if(count<128)
			{
				buffer[count]=key;
				count++;
			}
		}
	}
}








void f2init(QWORD world)
{
	dirbuffer=world+0x200000;
}