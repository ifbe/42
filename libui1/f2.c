#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void background2();

void string(int x,int y,char* str);
void hexadecimal(int x,int y,QWORD in);
void decimal(int x,int y,QWORD in);

QWORD whereisworld();




//
static int count=0;

static char buffer[128];
static char postfix[128];
static char stack[128];

static double result;

//
static QWORD dirbuffer;













//134+95*x+(70*44+f)*g ----> 134 95 x * + 70 44 * f + g * +
void infix2postfix(char* infix,char* postfix)
{
	//
	int source;
	int dest;
	int top;
	for(dest=0;dest<128;dest++)
	{
		stack[dest]=postfix[dest]=0;
	}

	//
	source=dest=0;
	top=128;			//只转换的话，栈里只有符号没有数字
	while(1)
	{
		switch( infix[source] )
		{
			case 0:		//结束了，把栈里能搬的全搬过去完事
			{
			while(1)
			{
				//栈已清空，返回
				if( top >= 128)return;

				//写一个分隔符，再搬出一个数字
				postfix[dest]=0x20;
				dest++;

				postfix[dest]=stack[top];
				dest++;

				//下一个
				top++;
			}
			}
			case '[':		//左括号无条件进栈
			{
				top--;
				stack[top]='[';
				break;
			}
			//case '^':		//把里面的%&换出去，否则进栈
			//case '%':
			case '*':		//把%^,*/换出去，否则进栈
			case '/':
			{
				postfix[dest]=0x20;
				dest++;

				if( (stack[top]=='*') | (stack[top]=='/') )
				{
					postfix[dest]=stack[top];
					dest++;
				}
				else top--;

				stack[top]=infix[source];
				break;
			}
			case '+':		//把%^,*/,+=换出去，否则进栈
			case '-':
			{
				postfix[dest]=0x20;
				dest++;

				if(top>=128) top--;
				else
				{
					if( (stack[top]=='*') | (stack[top]=='/') | (stack[top]=='+') | (stack[top]=='-') )
					{
						postfix[dest]=stack[top];                    //就先输出栈顶
						dest++;
					}
					else top--;                                    //直到是左括号或者空格
				}

				stack[top]=infix[source];                       //符号进栈
				break;
			}
			case ']':
			{
				while(stack[top]!='[')
				{
					postfix[dest]=0x20;
					dest++;

					postfix[dest]=stack[top];    //一直弹出直到遇到左括号或者空格
					dest++;

					top--;
				}

				top--;                    //弹出左括号扔掉
				break;
			}
			case 0x20:		//空格忽略掉
			{
				source++;
				break;
			}
			default:		//数字直接输出
			{
				postfix[dest]=infix[source];
				dest++;
				source++;
			}
		}//switch

		source++;

	}//while(1)
}




void calculator(char* postfix)
{
	int num=0;
	int top=0;
	int temp;
	double stack2[20];
	for(temp=0;temp<20;temp++){stack2[temp]=0.00;}

	while(num<127)
	{
		switch(postfix[num])
		{
			case 0x20:
			{
				if(postfix[num-1]>='0'&&postfix[num-1]<='9'){top++;}
				break;  //遇空格说明数字结束
			}
			case '.':
			{
				temp=1;
				num++;
				while(postfix[num]!=0x20)
				{
					temp*=10;
					stack2[top]+=(double)(postfix[num]-0x30)/(double)temp;
					num++;
				}
				top++;
				break;
			}
			case '+':
			{
				top--;
				stack2[top-1]+=stack2[top];
				stack2[top]=0.00;
				break;
			}
			case '-':
			{
				top--;
				stack2[top-1]-=stack2[top];
				stack2[top]=0.00;
				break;
			}
			case '*':
			{
				top--;
				stack2[top-1]*=stack2[top];
				stack2[top]=0.00;
				break;
			}
			case '/':
			{
				top--;
				if((stack2[top]>0.0001)||(stack2[top]<-0.0001))
				{
					stack2[top-1]/=stack2[top];
				}
				else
				{
					stack2[top-1]=1.7e+308;
				}
				stack2[top]=0.00;
				break;
			}
			default:
			{
				//如果是数字
				stack2[top]=10*stack2[top]+(double)(postfix[num]-0x30);
			}
		}
		num++;
	}

	result=stack2[0];
}










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
	background2();

	printfile0();

	string(0,0,buffer);
	string(0,1,postfix);
	decimal(0,2,(QWORD)result);
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
		else if(key==0xd)			//enter
		{
			infix2postfix(buffer,postfix);
			calculator(postfix);

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