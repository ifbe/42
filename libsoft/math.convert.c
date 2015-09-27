#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




static char stack[128];
static int count=0;




//空栈的时候rsp值最大:		rsp=0xa0000		(count=128)
//满栈的时候rsp值最小:		rsp=0x90000		(count=0)
static void initstack()
{
	count=128;
}

//push rax:
//rsp-8,[rsp]=rax		(count--,stack[count]=data)
static int push(char data)
{
	//满栈
	if(count==0)return 0;

	//count-1(rsp-8)，然后放下这个数字
	count--;
	stack[count]=data;
	return 1;
}

//pop rax:
//rax=[rsp],rsp+8		(data=stack[count],count++)
static int pop(char* dest)
{
	//空栈
	if(count>=128)return 0;

	//拿出当前数字，然后count+1(rsp+8)
	dest[0]=stack[count];
	count++;
	return 1;
}




static int operatorpriority(QWORD operator)
{
	if(operator == '(' ) return 0;

	else if(operator == '+' )return 1;
	else if(operator == '-' )return 1;

	else if(operator == '*' )return 2;
	else if(operator == '/' )return 2;
	else if(operator == '^' )return 2;
	else if(operator == '%' )return 2;
}




void infix2postfix(char* infix,char* postfix)
{
	//
	int source;
	int dest;

	char stacktop;
	int ret;
	int compareresult;

	initstack();

	//
	source=dest=0;
	while(1)
	{
		switch( infix[source] )
		{
			case '=':
			case 0:		//结束了，把栈里能搬的全搬过去完事
			{
				while(1)
				{
					ret=pop(postfix+dest);
					if(ret<=0)return;

					dest++;
				}
			}
			case '(':
			case '[':		//左括号无条件进栈
			{
				push( '(' );
				break;
			}
			case ')':
			case ']':
			{
				postfix[dest]=0x20;
				dest++;

				while(1)
				{
					//叫一个出来
					ret=pop(&stacktop);

					//出问题了
					if( ret<=0 )return;

					//是左括号的话直接扔掉，这个右括号就处理完了
					if( stacktop == '(' )break;

					//否则把刚叫出来的放走，继续叫下一个来处理
					else
					{
						postfix[dest]=stacktop;    //一直弹出直到遇到左括号或者空格
						dest++;
					}
				}

				break;
			}//右括号
			case '+':		//把%,^,/,*换出去，否则进栈
			case '-':
			case '*':
			case '/':
			case '^':		//乘方
			case '%':		//取余
			case '!':		//阶乘
			{
				postfix[dest]=0x20;
				dest++;

				while(1)
				{
					//叫一个出来跟自己比
					ret=pop(&stacktop);
					compareresult=operatorpriority(stacktop) - operatorpriority(infix[source]);

					//空栈直接放，比自己弱就让它滚回栈里，并且自己也进栈完事
					if( (ret<=0) | (compareresult<0) )
					{
						push( stacktop );
						push( infix[source] );
						break;
					}

					//跟自己一样弱鸡或者比自己强，就让它先走然后自己进入下次循环
					else
					{
						postfix[dest]=stacktop;
						dest++;
					}

				}//while

				break;
			}//+,-,*,/,%,^,log,sqrt,cos......
			case ' ':
			{
				break;
			}
			default:		//数字直接输出
			{
				postfix[dest]=infix[source];
				dest++;
				break;
			}
		}//switch

		source++;
		if(source>=128)return;

	}//while(1)
}
void infix2binarytree(char* infix,char* postfix)
{
	
}