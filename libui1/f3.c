#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void background3();

void string(int x,int y,char* str);
void hexadecimal(int x,int y,QWORD in);
void decimal(int x,int y,QWORD in);

double calculator(char* postfix);
void postfix2binarytree(char* postfix);
void infix2postfix(char* infix,char* postfix);
void double2decimalstring(double,char*);

void printmemory(char*,int);
QWORD whereisworld();








//
static int count=0;
static char buffer[128];

static char postfix[128];
static char result[128];




void f3show()
{
	background3();

	string(0,0,buffer);
	string(0,1,postfix);

	string(0,2,result);
}
void f3message(QWORD type,QWORD key)
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
			double haha;
			//134+95*x+(70*44+f)*g -> 134 95 x *+ 70 44 * f + g *+
			printmemory(buffer,128);
			infix2postfix(buffer,postfix);

			haha=calculator(postfix);
			double2decimalstring(haha,result);

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








void f3init(QWORD world)
{
}
