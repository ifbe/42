#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define __cos__ hex32('c','o','s',0)
#define __sin__ hex32('s','i','n',0)
void bintree_plant(void*);
void bintree_fell(void*);
void bintree_setleft(void*, void*, void*);
void bintree_setright(void*, void*, void*);
void* bintree_grow(void*);
//
int stack_generate(void*, int);
int stack_destory(void*);
int stack_push(void*, void*);
int stack_pop(void*, void*);
//
int decstr2double(u8*, double*);
//
double getcos(double);
double getsin(double);
double power(double, double);
//
void printmemory(void*, int);
void logtoall(void*, ...);




struct mathnode
{
	u16 left;
	u16 right;
	u16 up;
	u16 type;
	union
	{
		u64 integer;
		double floatpoint;
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




void postfix2binarytree(u8* src, u8* bintree)
{
	double float1, float2;
	int ret1, ret2, data;
	struct mathnode* parent;
	struct mathnode* lchild;
	struct mathnode* rchild;
	u8 stack[0x1000];

	bintree_plant(bintree);
	stack_generate(stack, 0x1000);

	while(1)
	{
		data = *src;
		//logtoall("%02x\n",data);

		if(data == 0)
		{
			stack_pop(stack, &parent);
			bintree_setright(bintree, bintree, parent);
			break;
		}

		//const
		else if((data>='0') && (data<='9'))
		{
			src += decstr2double(src , &float1);

			parent = bintree_grow(bintree);
			parent->left = 0;
			parent->right = 0;
			parent->up = 0;
			parent->type = '0';
			parent->floatpoint = float1;

			stack_push(stack, &parent);
		}

		//variable
		else if((data=='x')|(data=='y')|(data=='z'))
		{
			parent = bintree_grow(bintree);
			parent->left = 0;
			parent->right = 0;
			parent->up = 0;
			parent->type = data;
			parent->integer = 0;

			stack_push(stack, &parent);
			src++;
		}

		//symbol
		else if((data == '+') | (data == '-') |
			(data == '*') | (data == '/') |
			(data == '^') | (data == '%') |
			(data == '=') )
		{
			ret2 = stack_pop(stack, &rchild);
			ret1 = stack_pop(stack, &lchild);
			if(ret1<=0|ret2<=0)break;

			parent = bintree_grow(bintree);
			parent->up = 0;
			parent->type = data;

			bintree_setleft(bintree, parent, lchild);
			bintree_setright(bintree, parent, rchild);

			stack_push(stack, &parent);
			src++;
		}
		else src++;
	}

	//printmemory(bintree, 0x20*16);
	//printmemory(stack, 0x20*16);
	return;
}
