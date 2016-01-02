#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
double cosine(double);
double sine(double);
int decstring2data(BYTE* source,QWORD* data);
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
static double fpstack[20];
static int fpcount=0;
static QWORD stack[128];
static int sp=0;








//------------------------------------------------
void initfpstack()
{
	fpcount=20;
}
int pushfp(double data)
{
	//say("fppush %llf\n",data);

	//满栈
	if(fpcount==0)return 0;

	//fpcount-1(rsp-8)，然后放下这个数字
	fpcount--;
	fpstack[fpcount]=data;
	return 1;
}
int popfp(double* dest)
{
	//空栈
	if(fpcount>=20)return 0;

	//拿出当前数字，然后fpcount+1(rsp+8)
	dest[0]=fpstack[fpcount];
	fpcount++;

	//say("fppop %llf\n",dest[0]);
	return 1;
}




void initstack()
{
	//空栈的时候rsp值最大:		rsp=0xa0000		(sp=128)
	//满栈的时候rsp值最小:		rsp=0x90000		(sp=0)
	sp=128;
}
int push(DWORD data)
{
	//push rax:
	//rsp-8,[rsp]=rax		(sp--,stack[sp]=data)

	//满栈
	if(sp==0)return 0;

	//sp-1(rsp-8)，然后放下这个数字
	sp--;
	stack[sp]=data;
	return 1;
}
int pop(DWORD* dest)
{
	//pop rax:
	//rax=[rsp],rsp+8		(data=stack[sp],sp++)

	//空栈
	if(sp>=128)return 0;

	//拿出当前数字，然后sp+1(rsp+8)
	dest[0]=stack[sp];
	sp++;

	//diary("pop %d\n",dest[0]);
	return 1;
}




//-----------------------------------------------------------
static int operatorpriority(QWORD operator)
{
	if(operator == '(' ) return 0;

	else if(operator == '+' )return 1;
	else if(operator == '-' )return 1;

	else if(operator == '*' )return 2;
	else if(operator == '/' )return 2;
	else if(operator == '^' )return 2;
	else if(operator == '%' )return 2;

	else return 9;
}




//把混乱的输入变成整理好的式子
void mess2infix(char* in,char* out)
{
	//暂时不管，只复制不做事
	int i;
	for(i=0;i<127;i++)
	{
		out[i]=in[i];
	}
}




//中缀表达式转后缀表达式：134+95*x+(70*44+f)*g -> 134 95 x *+ 70 44 * f + g *+
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
			case '=':		//别急还有等式右边呢
			{
				while(1)
				{
					ret=pop(&stacktop);
					if(ret<=0)break;

					postfix[dest]=stacktop&0xff;
					dest++;
				}

				postfix[dest]='=';
				dest++;
				break;
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
			/*
			case 'c':	//cos
			{
				if(infix[source+1]=='0' && infix[source+2]=='s')
				{
					source+=2;
				}
				break;
			}
			*/
			//case 'l':	log
			//case 's'	sqrt
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




//后缀表达式转表达式二叉树：
void postfix2binarytree(char* postfix,struct mathnode* node)
{
	int source;
	int dest;
	int haha;

	QWORD data;
	double float1,float2;

	DWORD first,second;
	int ret1,ret2;




	//rsp=128
	initstack();

	//值为0代表算式如:"1+2*[9-6]",	非0代表等式如:"1+2*[9-6]=54.321/x^3"
	node[0].integer=0;




	//正式开始
	source=0;
	dest = 1;		//这一号用掉了
	while(1)
	{
		//结束符
		if( postfix[source] == 0 )
		{
			node[dest].type=0;

			pop(&first);
			node[0].right=first;
			break;
		}




		//等号
		if( postfix[source] == '=' )
		{
			//等号节点，for fun
                        node[dest].type='=';
                        node[dest].left=0;
                        node[dest].right=0;
                        node[dest].integer=0;

			//point zero to root
			pop(&first);

			node[0].type=0x3d3d3d3d;	//====
			node[0].left=first;
			node[0].integer=dest;		//等号的位置

			source++;
			dest++;
		}




		//第1种：常量
		else if( ( postfix[source] >= '0' ) && ( postfix[source] <= '9' ) )
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




	//debug
	printmemory((char*)node,0x20*16);
}




//-----------------------------------------------------------
double calculator(char* postfix,QWORD x,QWORD y)
{
	int source=0;
	int count;
	QWORD data;
	double first,second,temp;

	initfpstack();
	while(1)
	{
		//第1种：常量
		if( ( postfix[source] >= '0' ) && ( postfix[source] <= '9' ) )
		{
			//先拿整数部分
			count = decstring2data( postfix+source , &data );
			source += count;
			first = (double)data;

			//检查有没有小数部分有就加上
			if(postfix[source] == '.')
			{
				//say(".@%d\n",source);
				source++;
				count=decstring2data( postfix+source , &data );

				if(count>0)
				{
					source += count;
					temp = (double)data;

					while(1)
					{
						temp /= 10.00;

						count--;
						if(count==0)break;
					}

					//加上小数
					first+=temp;
				}

			}

			//保存这个double值
			pushfp(first);
		}//是数字




		//第2种：变量
		else if(postfix[source]=='x')
		{
			//pushfp((double)x);
			source++;
		}
		else if(postfix[source]=='y')
		{
			//pushfp((double)y);
			source++;
		}
		else if(postfix[source]=='z')
		{
			//pushfp((double)z);
			source++;
		}




		//第3种：单字节符号
		else if(postfix[source] == '+')
		{
			popfp(&second);
			popfp(&first);		//注意，栈，先进后出
			temp = first + second;
			pushfp(temp);

			source++;
		}
		else if(postfix[source] == '-')
		{
			popfp(&second);
			popfp(&first);
			temp=first-second;
			pushfp(temp);

			source++;
		}
		else if(postfix[source] == '*')
		{
			popfp(&second);
			popfp(&first);
			temp=first*second;
			pushfp(temp);

			source++;
		}
		else if(postfix[source] == '/')
		{
			popfp(&second);
			popfp(&first);
			temp=first/second;
			pushfp(temp);

			source++;
		}
		else if(postfix[source] == '^') 	//指数		x^y
		{
			popfp(&second);
			popfp(&first);

			temp=1.00;
			data=(QWORD)(second+0.000001);
			if(data!=0) while(1)
			{
				temp*=first;
				data--;
				if(data==0)break;
			}
			pushfp(temp);

			source++;
		}
		else if(postfix[source] == '%') 	//取余		x%y
		{
			source++;
		}
		else if(postfix[source] == '!') 	//阶乘		x!
		{
			source++;
		}




		//第4种：多字节符号
		else if(postfix[source] == 'l')		//对数		xlogy
		{
			source++;
		}
		else if(postfix[source] == 's')
		{
			//根号		ysqrty
			//正弦		sinx
			source++;		//下一个
		}
		else if(postfix[source] == 'c') 	//余弦		cosx
		{
			source++;		//下一个
		}
		else if(postfix[source] == 't') 	//正切		tanx
		{
			source++;		//下一个
		}
		else
		{
			source++;			//其他不认识的不管，不加会死这儿
		}




		//检查退出while循环
		if(source>=128)break;
		if(postfix[source]==0)break;




	}//while结束

	popfp(&temp);
	return temp;
}




double sketchpad(struct mathnode* node,double x,double y)
{
	int source=1;
	double first,second,temp;
	double result1,result2;

	result1=0;
	initfpstack();
	while(1)
	{
		if( node[source].type == 0x33323130 )	//0123...
		{
			pushfp(node[source].floatpoint);
		}
		else if( node[source].type == 'x' )
		{
			pushfp(x);
		}
		else if( node[source].type == 'y' )
		{
			pushfp(y);
		}
		else if( node[source].type == 0x2f2a2d2b )		//+-*/...
		{
			if( node[source].integer == '+' )
			{
				popfp(&second);
				popfp(&first);            //注意，栈，先进后出
				pushfp(first+second);
			}
			if( node[source].integer == '-' )
			{
				popfp(&second);
				popfp(&first);            //注意，栈，先进后出
				pushfp(first-second);
			}
			if( node[source].integer == '*' )
			{
				popfp(&second);
				popfp(&first);            //注意，栈，先进后出
				pushfp(first*second);
			}
			if( node[source].integer == '/' )
			{
				popfp(&second);
				popfp(&first);            //注意，栈，先进后出
				pushfp(first/second);
			}
		}
		else if( node[source].type == 0x736f63 )	//cos
		{
			popfp(&first);
			pushfp( cosine(first) );
		}
		else if( node[source].type == 0x6e6973 )	//sin
		{
			popfp(&first);
			pushfp( sine(first) );
		}
		else if( node[source].type == 0x3d)	//=
		{
			popfp(&result1);
		}
		else if(node[source].type == 0)
		{
			popfp(&result2);
			result2-=result1;
			break;
		}

		//
		source++;
	}

	return result2;
}
