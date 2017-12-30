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
void* bintree_getleft(void*, void*);
void* bintree_getright(void*, void*);
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




double sketchpad_one(
	struct mathnode* node, struct mathnode* this,
	double x, double y)
{
	u16 type;
	double a, b;
	struct mathnode* left;
	struct mathnode* right;

	type = this->type;
	if(type == '0')return this->floatpoint;
	if(type == 'x')return x;
	if(type == 'y')return y;

	left = bintree_getleft(node, this);
	right = bintree_getright(node, this);
	a = sketchpad_one(node, left, x, y);
	b = sketchpad_one(node, right, x, y);

	if(type == '+')return a+b;
	if(type == '-')return a-b;
	if(type == '*')return a*b;
	if(type == '/')return a/b;
	if(type == '=')return a-b;
}
double sketchpad(struct mathnode* node, double x, double y)
{
	struct mathnode* root;
	if(node == 0)return 0.0;

        root = bintree_getright(node, node);
	if(root == 0)return 0.0;

	return sketchpad_one(node, root, x, y);
}
