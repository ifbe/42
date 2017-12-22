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
