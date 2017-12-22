#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define __cos__ hex32('c','o','s',0)
#define __sin__ hex32('s','i','n',0)
int stack_generate(void*, int);
int stack_destory(void*);
int stack_push(void*, void*);
int stack_pop(void*, void*);
//
int hexstr2data(u8*, u64*);
int decstr2data(u8*, u64*);
int decstr2double(u8*, double*);
//
double cosine(double);
double sine(double);
double power(double, double);
//
void printmemory(void*, int);
void say(void*, ...);




struct mathnode
{
	u16 left;
	u16 right;
	u16 up;
	u16 type;
	union
	{
		double floatpoint;
		unsigned long long integer;
	};
};




static int priority(u64 operator)
{
	if(operator == '(') return 0;

	else if(operator == '=')return 1;

	else if(operator == '+')return 2;
	else if(operator == '-')return 2;

	else if(operator == '*')return 3;
	else if(operator == '/')return 3;
	else if(operator == '^')return 3;
	else if(operator == '%')return 3;

	else return 9;
}




//中缀表达式转后缀表达式：134+95*x+(70*44+f)*g -> 134 95 x * + 70 44 * f + g * +
void infix2postfix(u8* src, u8* dst)
{
	int ret;
	u64 top, this;
	u8 stack[0x1000];
	stack_generate(stack, 0x1000);

	while(1)
	{
		this = *src;
		if(this == 0)break;

		else if( (this == '(') | (this == '[') )
		{
			this = '(';
			stack_push(stack, &this);
		}
		else if( (this == ')') | (this == ']') )
		{
			*dst = 0x20;
			dst++;

			while(1)
			{
				ret = stack_pop(stack, &top);
				if(ret == 0)break;
				if(top == '(')break;

				*dst = top;
				dst++;
			}
		}
		else if((this == '+') | (this == '-') |
			(this == '*') | (this == '/') |
			(this == '^') | (this == '%') |
			(this == '=') )
		{
			*dst = 0x20;
			dst++;

			while(1)
			{
				ret = stack_pop(stack, &top);
				if(ret == 0)
				{
					this = *src;
					stack_push(stack, &this);
					break;
				}

				ret = priority(top) - priority(*src);
				if(ret < 0)
				{
					stack_push(stack, &top);
					this = *src;
					stack_push(stack, &this);
					break;
				}
				else
				{
					*dst = top;
					dst++;
				}
			}//while
		}
		else if(this>0x20)
		{
			*dst = this;
			dst++;
		}

		src++;

	}//while(1)

	while(1)
	{
		ret = stack_pop(stack, &top);
		if(ret == 0)break;

		*dst = top&0xff;
		dst++;
	}
	*dst = 0;
	return;
}
double calculator(u8* postfix, u64 x, u64 y, u64 z)
{
	int src=0;
	double first,second,temp;
	u8 stack[0x1000];
	stack_generate(stack, 0x1000);

	while(1)
	{
		//第1种：常量
		if( ( postfix[src] >= '0' ) && ( postfix[src] <= '9' ) )
		{
			src += decstr2double(postfix+src , &first);
			stack_push(stack, &first);
		}//是数字




		//第2种：变量
		else if(postfix[src]=='x')
		{
			//temp = (double)x;
			//stack_push(stack, &temp);
			src++;
		}
		else if(postfix[src]=='y')
		{
			//temp = (double)y;
			//stack_push(stack, &temp);
			src++;
		}
		else if(postfix[src]=='z')
		{
			//temp = (double)z;
			//stack_push(stack, &temp);
			src++;
		}




		//第3种：单字节符号
		else if(postfix[src] == '+')
		{
			stack_pop(stack, &second);
			stack_pop(stack, &first);
			temp = first + second;
			stack_push(stack, &temp);

			src++;
		}
		else if(postfix[src] == '-')
		{
			stack_pop(stack, &second);
			stack_pop(stack, &first);
			temp = first - second;
			stack_push(stack, &temp);

			src++;
		}
		else if(postfix[src] == '*')
		{
			stack_pop(stack, &second);
			stack_pop(stack, &first);
			temp = first * second;
			stack_push(stack, &temp);

			src++;
		}
		else if(postfix[src] == '/')
		{
			stack_pop(stack, &second);
			stack_pop(stack, &first);
			temp = first / second;
			stack_push(stack, &temp);

			src++;
		}
		else if(postfix[src] == '^') 	//指数		x^y
		{
			stack_pop(stack, &second);
			stack_pop(stack, &first);
			temp = power(first, second);
			stack_push(stack, &temp);

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
		else if(postfix[src] == 'l')	//对数	xlogy
		{
			src++;
		}
		else if(postfix[src] == 's')
		{
			//根号		ysqrty
			//正弦		sinx
			src++;		//下一个
		}
		else if(postfix[src] == 'c') 	//余弦	cosx
		{
			src++;		//下一个
		}
		else if(postfix[src] == 't') 	//正切	tanx
		{
			src++;		//下一个
		}
		else src++;




		//检查退出while循环
		if(src >= 128)break;
		if(postfix[src] == 0)break;




	}//while结束

	stack_pop(stack, &temp);
	return temp;
}








//后缀表达式转表达式二叉树：
void postfix2binarytree(u8* postfix,struct mathnode* node)
{
	int ret1, ret2, haha;
	u64 src, dst, data;
	u64 first, second;
	double float1, float2;

	u8 stack[0x1000];
	stack_generate(stack, 0x1000);




	//值为0代表算式如:"1+2*[9-6]",	非0代表等式如:"1+2*[9-6]=54.321/x^3"
	node[0].integer=0;
	src = 0;
	dst = 1;
	while(1)
	{
		//结束符
		if( postfix[src] == 0)
		{
			node[dst].type=0;

			stack_pop(stack, &first);
			node[0].right=first;
			break;
		}




		//等号
		if( postfix[src] == '=')
		{
			//等号节点，for fun
			node[dst].type='=';
			node[dst].left=0;
			node[dst].right=0;
			node[dst].integer=0;

			//point zero to root
			stack_pop(stack, &first);

			node[0].type = '=';
			node[0].left = first;
			node[0].integer = dst;

			src++;
			dst++;
		}




		//第1种：常量
		else if( (postfix[src] >= '0') && (postfix[src] <= '9') )
		{
			//say("herehere!!!!\n");
			//先拿整数部分
			src += decstr2double(postfix+src , &float1);

			//造节点
			node[dst].type = '0';
			node[dst].left = 0;
			node[dst].right = 0;
			node[dst].floatpoint=float1;

			//保存节点号
			stack_push(stack, &dst);
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

			stack_push(stack, &dst);
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
				if(postfix[src+1]=='o' && postfix[src+2]=='g')
				{
					data=0x676f6c;
					src+=2;		//3-1
				}
				else haha=0;
			}
			else if(postfix[src]=='s')
			{
				if(*(u32*)(postfix+src) == 0x74727173)
				{
					data=0x74727173;	//sqrt
					src+=3;		//4-1
					haha=1;
				}
				else if(postfix[src+1]=='i' && postfix[src+2]=='n')
				{
					data=0x6e6973;
					src+=2;		//3-1
					haha=1;
				}
				else haha=0;
			}
			else if(postfix[src]=='c')
			{
				if(postfix[src+1]=='o' && postfix[src+2]=='s')
				{
					data=0x736f63;
					src+=2;		//3-1
					haha=1;
				}
				else haha=0;
			}
			else if(postfix[src]=='t')
			{
				if(postfix[src+1]=='a' && postfix[src+2]=='n')
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
					ret2 = stack_pop(stack, &second);
					ret1 = stack_pop(stack, &first);
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
					stack_pop(stack, &first);
					if(first>=128)break;

					//处理
					node[first].up=dst;
					node[dst].left=first;
					node[dst].right=0;
				}

				node[dst].type = '+';
				node[dst].integer = data;
				stack_push(stack, &dst);
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




double sketchpad(struct mathnode* node,double x,double y)
{
	int src=1;
	double first,second,temp;
	double result1,result2;
	u8 stack[0x1000];
	stack_generate(stack, 0x1000);

	result1=0;
	while(1)
	{
		if(node[src].type == 0)
		{
			stack_pop(stack, &result2);
			result2 -= result1;
			break;
		}
		else if(node[src].type == '0')	//0123...
		{
			temp = node[src].floatpoint;
			stack_push(stack, &temp);
		}
		else if(node[src].type == 'x')
		{
			stack_push(stack, &x);
		}
		else if(node[src].type == 'y')
		{
			stack_push(stack, &y);
		}
		else if(node[src].type == '+')
		{
			if(node[src].integer == '+')
			{
				stack_pop(stack, &second);
				stack_pop(stack, &first);
				temp = first+second;
				stack_push(stack, &temp);
			}
			else if(node[src].integer == '-')
			{
				stack_pop(stack, &second);
				stack_pop(stack, &first);
				temp = first-second;
				stack_push(stack, &temp);
			}
			else if(node[src].integer == '*')
			{
				stack_pop(stack, &second);
				stack_pop(stack, &first);
				temp = first*second;
				stack_push(stack, &temp);
			}
			else if(node[src].integer == '/')
			{
				stack_pop(stack, &second);
				stack_pop(stack, &first);
				temp = first/second;
				stack_push(stack, &temp);
			}
			else if(node[src].integer == '^')
			{
				stack_pop(stack, &second);
				stack_pop(stack, &first);
				temp = power(first, second);
				stack_push(stack, &temp);
			}
			else if(node[src].integer == __cos__)
			{
				stack_pop(stack, &first);
				temp = cosine(first);
				stack_push(stack, &temp);
			}
			else if(node[src].integer == __sin__)
			{
				stack_pop(stack, &first);
				temp = sine(first);
				stack_push(stack, &temp);
			}
		}
		else if(node[src].type == 0x3d)	//=
		{
			stack_pop(stack, &result1);
		}

		//
		src++;
	}

	return result2;
}
