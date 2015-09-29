#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
int decstring2data(char*,QWORD*);
void printmemory(char*,int);
void diary(char*,...);




struct mathnode{

	DWORD type;
	DWORD up;
	DWORD left;
	DWORD right;
	union{
		char datasize[16];
		double floatpoint;
		unsigned long long integer;
	};
};




static struct mathnode node[128];
static DWORD stack[128];
static int sp=0;




//空栈的时候rsp值最大:		rsp=0xa0000		(sp=128)
//满栈的时候rsp值最小:		rsp=0x90000		(sp=0)
static void initstack()
{
	sp=128;
}
//push rax:
//rsp-8,[rsp]=rax		(sp--,stack[sp]=data)
static int push(DWORD data)
{
	diary("push %d\n",data);

	//满栈
	if(sp==0)return 0;

	//sp-1(rsp-8)，然后放下这个数字
	sp--;
	stack[sp]=data;
	return 1;
}
//pop rax:
//rax=[rsp],rsp+8		(data=stack[sp],sp++)
static int pop(DWORD* dest)
{
	//空栈
	if(sp>=128)return 0;

	//拿出当前数字，然后sp+1(rsp+8)
	dest[0]=stack[sp];
	sp++;

	diary("pop %d\n",dest[0]);
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

	int ret;
	int compareresult;

	DWORD stacktop;

	initstack();
	for(ret=0;ret<128;ret++)postfix[ret]=0;

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
					ret=pop(&stacktop);
					if(ret<=0)goto theend;

					postfix[dest]=stacktop&0xff;
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
					if( ret<=0 )goto theend;

					//是左括号的话直接扔掉，这个右括号就处理完了
					if( stacktop == '(' )break;

					//否则把刚叫出来的放走，继续叫下一个来处理
					else
					{
						postfix[dest]=stacktop;    //直到遇到左括号或者空格
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
					//叫一个出来跟自己比，空栈就自己进去
					ret=pop(&stacktop);
					if(ret==0)
					{
						push( infix[source] );
						break;
					}
					else
					{
						compareresult=operatorpriority(stacktop) - operatorpriority(infix[source]);
						//比自己弱就让它滚回栈里，并且自己也进栈完事
						if(compareresult<0)
						{
							push( stacktop );
							push( infix[source] );
							break;
						}

						//否则让它先走然后自己进入下次循环
						else
						{
							postfix[dest]=stacktop;
							dest++;
						}
					}

				}//while

				break;
			}//+,-,*,/,%,^
			//case 'l':	log
			//case 's'	sqrt
			//case 'c'	cos
			case ' ':
			{
				break;
			}
			default:		//数字直接输出
			{
				if(infix[source]>0x20)
				{
					postfix[dest]=infix[source];
					dest++;
				}
				break;
			}
		}//switch

		source++;
		if(source>=128)goto theend;

	}//while(1)

theend:
	printmemory(postfix,128);
}








void postfix2binarytree(char* postfix,struct mathnode** out)
{
	int source;
	int dest;
	int haha;

	QWORD data;
	double float1,float2;

	DWORD first,second;
	int ret1,ret2;




	//init
	initstack();




	//正式开始
	source=0;
	dest = 1;		//这一号用掉了
	while(1)
	{
		//第1种：常量
		if( ( postfix[source] >= '0' ) && ( postfix[source] <= '9' ) )
		{
			//diary("herehere!!!!\n");
			//先拿整数部分
			ret1 = decstring2data( postfix + source , &data );
			source += ret1;
			float1=(double)data;

			//如果有小数部分再拿上
			if(postfix[source] == '.')
			{
				//say(".@%d\n",source);
				source++;
				ret1=decstring2data( postfix+source , &data );

				if(ret1>0)
				{
					source += ret1;
					float2 = (double)data;

					while(1)
					{
						float2 /= 10.00;

						ret1--;
						if(ret1==0)break;
					}

					//加上小数
					float1+=float2;
				}

			}

			//造节点
			node[dest].type=0x33323130;
			node[dest].left=0;
			node[dest].right=0;
			node[dest].floatpoint=float1;

			//保存节点号
			push(dest);
			dest++;		//这一号用掉了
		}




		//第2种：变量
		else if( (postfix[source]=='x')|(postfix[source]=='y')|(postfix[source]=='z'))
		{
			//造节点
			node[dest].type=postfix[source];
			node[dest].left=0;
			node[dest].right=0;
			node[dest].integer=0;

			push(dest);
			dest++;

			//下一个
			source++;
		}




		//第3种：符号
		else
		{
			//0：不正常，1：单操作数，2：默认双操作数
			haha=2;

			if(postfix[source]=='+')data='+';
			else if(postfix[source]=='-')data='-';
			else if(postfix[source]=='*')data='*';
			else if(postfix[source]=='/')data='/';
			else if(postfix[source]=='^')data='^';
			else if(postfix[source]=='%')data='%';
			else if(postfix[source]=='!')
			{
				data='!';
				haha=1;
			}
			else if(postfix[source]=='l')
			{
				if( postfix[source+1]=='o' && postfix[source+2]=='g' )
				{
					data=0x676f6c;
					source+=2;		//3-1
				}
				else haha=0;
			}
			else if(postfix[source]=='s')
			{
				if( *(DWORD*)(postfix+source) == 0x74727173 )
				{
					data=0x74727173;	//sqrt
					source+=3;		//4-1
					haha=1;
				}
				else if( postfix[source+1]=='i' && postfix[source+2]=='n' )
				{
					data=0x6e6973;
					source+=2;		//3-1
					haha=1;
				}
				else haha=0;
			}
			else if(postfix[source]=='c')
			{
				if( postfix[source+1]=='o' && postfix[source+2]=='s' )
				{
					data=0x736f63;
					source+=2;		//3-1
					haha=1;
				}
				else haha=0;
			}
			else if(postfix[source]=='t')
			{
				if( postfix[source+1]=='a' && postfix[source+2]=='n' )
				{
					data=0x6e6174;
					source+=2;		//3-1
					haha=1;
				}
				else haha=0;
			}
			else haha=0;




			//这是符号，所以对应的2个或1个数字可以滚了
			if(haha>0)
			{
				//diary("haha:%d\n",haha);
				if(haha == 2)
				{
					//拿出两个并且检查出错没有
					ret2=pop(&second);		//后进先出
					ret1=pop(&first);
					//diary("ret1=%x,first=%d\n",ret1,first);
					//diary("ret2=%x,second=%d\n",ret2,second);

					if(ret1<=0|ret2<=0)break;
					if(second>=128|first>=128)break;

					//处理
					node[first].up=dest;
					node[second].up=dest;
					node[dest].left=first;
					node[dest].right=second;
				}
				else if(haha == 1)
				{
					//拿出一个并且检查出错没有
					pop(&first);
					if(first>=128)break;

					//处理
					node[first].up=dest;
					node[dest].left=first;
					node[dest].right=0;
				}

				node[dest].type=0x2f2a2d2b;
				node[dest].integer=data;
				push(dest);
				dest++;
			}

			//不管正不正常，都会做这些事
			source++;

		}//else

		//postfix字符串最大长度
		if(source>=128)break;
		//if(postfix[source]==0)break;

	}//while(1)




	//point zero to root
	pop(&first);
	node[0].type=0x21212121;
	node[0].up=0;
	node[0].left=first;
	node[0].right=first;
	node[0].integer=first;

	//debug
	printmemory((char*)node,0x20*16);
	out[0]=node;
}
