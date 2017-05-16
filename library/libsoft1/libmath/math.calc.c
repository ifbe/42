#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int hexstr2data(u8*,u64*);
int decstr2data(u8*,u64*);
//
double cosine(double);
double sine(double);
//
void printmemory(void*, int);
void say(void*, ...);




struct mathnode{

	u32 type;
	u32 up;
	u32 left;
	u32 right;
	union{
		u8 datasize[16];
		double floatpoint;
		unsigned long long integer;
	};
};
//
static double fpstack[20];
static int fpcount=0;
static u64 stack[128];
static int sp=0;
//
static u8* guys;
static u8* fshome;
static u8* dirhome;
static u8* datahome;








//------------------------------------------------
void createfpstack()
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
int popfp(double* dst)
{
	//空栈
	if(fpcount>=20)return 0;

	//拿出当前数字，然后fpcount+1(rsp+8)
	dst[0]=fpstack[fpcount];
	fpcount++;

	//say("fppop %llf\n",dst[0]);
	return 1;
}




void createstack()
{
	//空栈的时候rsp值最大:		rsp=0xa0000		(sp=128)
	//满栈的时候rsp值最小:		rsp=0x90000		(sp=0)
	sp=128;
}
int push(u32 data)
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
int pop(u32* dst)
{
	//pop rax:
	//rax=[rsp],rsp+8		(data=stack[sp],sp++)

	//空栈
	if(sp>=128)return 0;

	//拿出当前数字，然后sp+1(rsp+8)
	dst[0]=stack[sp];
	sp++;

	//say("pop %d\n",dst[0]);
	return 1;
}




//-----------------------------------------------------------
static int priority(u64 operator)
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




//中缀表达式转后缀表达式：134+95*x+(70*44+f)*g -> 134 95 x * + 70 44 * f + g * +
void infix2postfix(u8* infix, u8* postfix)
{
	//
	int src, dst, ret;
	u32 top, this;

	createstack();
	src=dst=0;

	while(1)
	{
		this = infix[src];
		if(this == 0)
		{
			while(1)
			{
				ret=pop(&top);
				if(ret<=0)goto theend;

				postfix[dst]=top&0xff;
				dst++;
			}
		}

		//如果这是个等式
		else if(this == '=')
		{
			while(1)
			{
				ret=pop(&top);
				if(ret<=0)break;

				postfix[dst]=top&0xff;
				dst++;
			}

			postfix[dst]='=';
			dst++;
		}

		//左括号无条件进栈
		else if( (this == '(') | (this == '[') )
		{
			push( '(' );
		}

		//右括号
		else if( (this == ')') | (this == ']') )
		{
			postfix[dst]=0x20;
			dst++;

			while(1)
			{
				//叫一个出来
				ret=pop(&top);

				//出问题了
				if( ret<=0 )goto theend;

				//是左括号的话直接扔掉，这个右括号就处理完了
				if( top == '(' )break;

				//否则把刚叫出来的放走，继续叫下一个来处理
				else
				{
					postfix[dst]=top;    //直到‘(’或者‘ ’
					dst++;
				}
			}
		}//右括号

/*
		else if(this == '!')		//阶乘
		{
		}

		else if(this == 'c')	//cos(
		{
			if(infix[src+1]=='0' && infix[src+2]=='s')
			{
				src+=2;
			}
		}

		else if(this == 'l')	//log(
		{
		}

		else if(this == 's')	//sqrt(
		{
		}
*/



		//
		else if((this == '+') |
			(this == '-') |
			(this == '*') |
			(this == '/') |
			(this == '^') |
			(this == '%') )
		{
			postfix[dst]=0x20;
			dst++;

			while(1)
			{
				//叫一个出来跟自己比，空栈就自己进去
				ret=pop(&top);
				if(ret==0)
				{
					push( infix[src] );
					break;
				}
				else
				{
					//比一下优先级
					ret = priority(top) - priority(infix[src]);

					//比自己弱就让它滚回栈里，并且自己也进栈完事
					if(ret < 0)
					{
						push( top );
						push( infix[src] );
						break;
					}

					//否则让它先走然后自己进入下次循环
					else
					{
						postfix[dst]=top;
						dst++;
					}
				}

			}//while
		}//+,-,*,/,%,^

		//数字直接输出
		else if(this>0x20)
		{
			postfix[dst] = this;
			dst++;
		}


		//next
		src++;
		if(src>=128)goto theend;
	}//while(1)

theend:
	//printmemory(postfix,128);
	postfix[dst] = 0;
	return;
}




//后缀表达式转表达式二叉树：
void postfix2binarytree(u8* postfix,struct mathnode* node)
{
	int src;
	int dst;
	int haha;

	u64 data;
	double float1,float2;

	u32 first,second;
	int ret1,ret2;




	//rsp=128
	createstack();

	//值为0代表算式如:"1+2*[9-6]",	非0代表等式如:"1+2*[9-6]=54.321/x^3"
	node[0].integer=0;




	//正式开始
	src=0;
	dst = 1;		//这一号用掉了
	while(1)
	{
		//结束符
		if( postfix[src] == 0 )
		{
			node[dst].type=0;

			pop(&first);
			node[0].right=first;
			break;
		}




		//等号
		if( postfix[src] == '=' )
		{
			//等号节点，for fun
			node[dst].type='=';
			node[dst].left=0;
			node[dst].right=0;
			node[dst].integer=0;

			//point zero to root
			pop(&first);

			node[0].type=0x3d3d3d3d;	//====
			node[0].left=first;
			node[0].integer=dst;		//等号的位置

			src++;
			dst++;
		}




		//第1种：常量
		else if( ( postfix[src] >= '0' ) && ( postfix[src] <= '9' ) )
		{
			//say("herehere!!!!\n");
			//先拿整数部分
			ret1 = decstr2data( postfix + src , &data );
			src += ret1;
			float1=(double)data;

			//如果有小数部分再拿上
			if(postfix[src] == '.')
			{
				//say(".@%d\n",src);
				src++;
				ret1=decstr2data( postfix+src , &data );

				if(ret1>0)
				{
					src += ret1;
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
			node[dst].type=0x33323130;
			node[dst].left=0;
			node[dst].right=0;
			node[dst].floatpoint=float1;

			//保存节点号
			push(dst);
			dst++;		//这一号用掉了
		}




		//第2种：变量
		else if( (postfix[src]=='x')|(postfix[src]=='y')|(postfix[src]=='z'))
		{
			//造节点
			node[dst].type=postfix[src];
			node[dst].left=0;
			node[dst].right=0;
			node[dst].integer=0;

			push(dst);
			dst++;

			//下一个
			src++;
		}




		//第3种：符号
		else
		{
			//0：不正常，1：单操作数，2：默认双操作数
			haha=2;

			if(postfix[src]=='+')data='+';
			else if(postfix[src]=='-')data='-';
			else if(postfix[src]=='*')data='*';
			else if(postfix[src]=='/')data='/';
			else if(postfix[src]=='^')data='^';
			else if(postfix[src]=='%')data='%';
			else if(postfix[src]=='!')
			{
				data='!';
				haha=1;
			}
			else if(postfix[src]=='l')
			{
				if( postfix[src+1]=='o' && postfix[src+2]=='g' )
				{
					data=0x676f6c;
					src+=2;		//3-1
				}
				else haha=0;
			}
			else if(postfix[src]=='s')
			{
				if( *(u32*)(postfix+src) == 0x74727173 )
				{
					data=0x74727173;	//sqrt
					src+=3;		//4-1
					haha=1;
				}
				else if( postfix[src+1]=='i' && postfix[src+2]=='n' )
				{
					data=0x6e6973;
					src+=2;		//3-1
					haha=1;
				}
				else haha=0;
			}
			else if(postfix[src]=='c')
			{
				if( postfix[src+1]=='o' && postfix[src+2]=='s' )
				{
					data=0x736f63;
					src+=2;		//3-1
					haha=1;
				}
				else haha=0;
			}
			else if(postfix[src]=='t')
			{
				if( postfix[src+1]=='a' && postfix[src+2]=='n' )
				{
					data=0x6e6174;
					src+=2;		//3-1
					haha=1;
				}
				else haha=0;
			}
			else haha=0;




			//这是符号，所以对应的2个或1个数字可以滚了
			if(haha>0)
			{
				//say("haha:%d\n",haha);
				if(haha == 2)
				{
					//拿出两个并且检查出错没有
					ret2=pop(&second);	//后进先出
					ret1=pop(&first);
					//say("ret1=%x,first=%d\n",ret1,first);
					//say("ret2=%x,second=%d\n",ret2,second);

					if(ret1<=0|ret2<=0)break;
					if(second>=128|first>=128)break;

					//处理
					node[first].up=dst;
					node[second].up=dst;
					node[dst].left=first;
					node[dst].right=second;
				}
				else if(haha == 1)
				{
					//拿出一个并且检查出错没有
					pop(&first);
					if(first>=128)break;

					//处理
					node[first].up=dst;
					node[dst].left=first;
					node[dst].right=0;
				}

				node[dst].type=0x2f2a2d2b;
				node[dst].integer=data;
				push(dst);
				dst++;
			}

			//不管正不正常，都会做这些事
			src++;

		}//else

		//postfix字符串最大长度
		if(src>=128)break;
		//if(postfix[src]==0)break;

	}//while(1)




	//debug
	//printmemory(node,0x20*16);
	return;
}




//-----------------------------------------------------------
double calculator(u8* postfix,u64 x,u64 y)
{
	int src=0;
	int count;
	u64 data;
	double first,second,temp;

	createfpstack();
	while(1)
	{
		//第1种：常量
		if( ( postfix[src] >= '0' ) && ( postfix[src] <= '9' ) )
		{
			//先拿整数部分
			count = decstr2data( postfix+src , &data );
			src += count;
			first = (double)data;

			//检查有没有小数部分有就加上
			if(postfix[src] == '.')
			{
				//say(".@%d\n",src);
				src++;
				count=decstr2data( postfix+src , &data );

				if(count>0)
				{
					src += count;
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
		else if(postfix[src]=='x')
		{
			//pushfp((double)x);
			src++;
		}
		else if(postfix[src]=='y')
		{
			//pushfp((double)y);
			src++;
		}
		else if(postfix[src]=='z')
		{
			//pushfp((double)z);
			src++;
		}




		//第3种：单字节符号
		else if(postfix[src] == '+')
		{
			popfp(&second);
			popfp(&first);		//注意，栈，先进后出
			temp = first + second;
			pushfp(temp);

			src++;
		}
		else if(postfix[src] == '-')
		{
			popfp(&second);
			popfp(&first);
			temp=first-second;
			pushfp(temp);

			src++;
		}
		else if(postfix[src] == '*')
		{
			popfp(&second);
			popfp(&first);
			temp=first*second;
			pushfp(temp);

			src++;
		}
		else if(postfix[src] == '/')
		{
			popfp(&second);
			popfp(&first);
			temp=first/second;
			pushfp(temp);

			src++;
		}
		else if(postfix[src] == '^') 	//指数		x^y
		{
			popfp(&second);
			popfp(&first);

			temp=1.00;
			data=(u64)(second+0.000001);
			if(data!=0) while(1)
			{
				temp*=first;
				data--;
				if(data==0)break;
			}
			pushfp(temp);

			src++;
		}
		else if(postfix[src] == '%') 	//取余		x%y
		{
			src++;
		}
		else if(postfix[src] == '!') 	//阶乘		x!
		{
			src++;
		}




		//第4种：多字节符号
		else if(postfix[src] == 'l')		//对数		xlogy
		{
			src++;
		}
		else if(postfix[src] == 's')
		{
			//根号		ysqrty
			//正弦		sinx
			src++;		//下一个
		}
		else if(postfix[src] == 'c') 	//余弦		cosx
		{
			src++;		//下一个
		}
		else if(postfix[src] == 't') 	//正切		tanx
		{
			src++;		//下一个
		}
		else
		{
			src++;			//其他不认识的不管，不加会死这儿
		}




		//检查退出while循环
		if(src>=128)break;
		if(postfix[src]==0)break;




	}//while结束

	popfp(&temp);
	return temp;
}




double sketchpad(struct mathnode* node,double x,double y)
{
	int src=1;
	double first,second,temp;
	double result1,result2;

	result1=0;
	createfpstack();
	while(1)
	{
		if( node[src].type == 0x33323130 )	//0123...
		{
			pushfp(node[src].floatpoint);
		}
		else if( node[src].type == 'x' )
		{
			pushfp(x);
		}
		else if( node[src].type == 'y' )
		{
			pushfp(y);
		}
		else if( node[src].type == 0x2f2a2d2b )		//+-*/...
		{
			if( node[src].integer == '+' )
			{
				popfp(&second);
				popfp(&first);	    //注意，栈，先进后出
				pushfp(first+second);
			}
			if( node[src].integer == '-' )
			{
				popfp(&second);
				popfp(&first);	    //注意，栈，先进后出
				pushfp(first-second);
			}
			if( node[src].integer == '*' )
			{
				popfp(&second);
				popfp(&first);	    //注意，栈，先进后出
				pushfp(first*second);
			}
			if( node[src].integer == '/' )
			{
				popfp(&second);
				popfp(&first);	    //注意，栈，先进后出
				pushfp(first/second);
			}
		}
		else if( node[src].type == 0x736f63 )	//cos
		{
			popfp(&first);
			pushfp( cosine(first) );
		}
		else if( node[src].type == 0x6e6973 )	//sin
		{
			popfp(&first);
			pushfp( sine(first) );
		}
		else if( node[src].type == 0x3d)	//=
		{
			popfp(&result1);
		}
		else if(node[src].type == 0)
		{
			popfp(&result2);
			result2-=result1;
			break;
		}

		//
		src++;
	}

	return result2;
}




static int calc_read(u8* p)
{
	return 0;
}
static int calc_write(u8* p)
{
	double ans; 
	u8* postfix = datahome;

	infix2postfix(p, postfix);
	say("postfix:%s\n", postfix);

	ans = calculator(postfix, 0, 0);
	say("answer:%f\n", ans);

	return 0;
}
static int calc_list(u8* p)
{
	return 0;
}
static int calc_choose(u8* p)
{
	return 0;
}
static int calc_start(u8* p)
{
	return 0;
}
static int calc_stop(u8* p)
{
	return 0;
}
void calc_create(u8* softaddr, u64* p)
{
	//
	guys = softaddr;
	fshome = softaddr+0x100000;
	dirhome = softaddr+0x200000;
	datahome = softaddr+0x300000;

	//
	p[0]=0x6874616d;
	p[1]=0x636c6163;
	p[2]=(u64)calc_start;
	p[3]=(u64)calc_stop;
	p[4]=(u64)calc_list;
	p[5]=(u64)calc_choose;
	p[6]=(u64)calc_read;
	p[7]=(u64)calc_write;
}
void calc_delete()
{
}
