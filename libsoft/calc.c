static char stack[128];




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
			case '(':
			case '[':		//左括号无条件进栈
			{
				top--;
				stack[top]=infix[source];

				break;
			}
			//case '^':		//把里面的%&换出去，否则进栈
			//case '%':
			case '*':		//把%,^,/,*换出去，否则进栈
			case '/':
			{
				postfix[dest]=0x20;
				dest++;

				while(1)
				{
					//空栈直接放
					if(top>=128)
					{
						top--;
						stack[top]=infix[source];
						break;
					}

					//已经是
					if( (stack[top]!='*') && (stack[top]!='/') )
					{
						top--;
						stack[top]=infix[source];
						break;
					}
					else
					{
						postfix[dest]=stack[top];
						dest++;
						top++;
					}
				}

				postfix[dest]=0x20;
				dest++;
				break;
			}
			case '+':		//把%^,*/,+=换出去，否则进栈
			case '-':
			{
				//printf("this=%c,stack[top]=%c\n",infix[source],stack[top]);
				postfix[dest]=0x20;
				dest++;

				while(1)
				{
					//空栈直接放
					if(top>=128)
					{
						top--;
						stack[top]=infix[source];
						break;
					}

					//
					if( (stack[top]!='*') && (stack[top]!='/') && (stack[top]!='+') && (stack[top]!='-') )
					{
						top--;
						stack[top]=infix[source];
						break;
					}
					else
					{
						postfix[dest]=stack[top];
						dest++;
						top++;
					}
				}
				postfix[dest]=0x20;
				dest++;
				break;
			}
			case ')':
			case ']':
			{
				while(1)
				{
					if(top>=128)return;		//errror

					if( stack[top]!='[' && stack[top]!='(' )
					{
						postfix[dest]=0x20;
						dest++;

						postfix[dest]=stack[top];    //一直弹出直到遇到左括号或者空格
						dest++;

						top++;
					}
					else break;
				}

				top++;                    //弹出左括号扔掉
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

	}//while(1)
}
double calculator(char* postfix)
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

	return stack2[0];
}