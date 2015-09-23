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
	top=128;			//ֻת���Ļ���ջ��ֻ�з���û������
	while(1)
	{
		switch( infix[source] )
		{
			case 0:		//�����ˣ���ջ���ܰ��ȫ���ȥ����
			{
			while(1)
			{
				//ջ����գ�����
				if( top >= 128)return;

				//дһ���ָ������ٰ��һ������
				postfix[dest]=0x20;
				dest++;

				postfix[dest]=stack[top];
				dest++;

				//��һ��
				top++;
			}
			}
			case '(':
			case '[':		//��������������ջ
			{
				top--;
				stack[top]=infix[source];

				break;
			}
			//case '^':		//�������%&����ȥ�������ջ
			//case '%':
			case '*':		//��%,^,/,*����ȥ�������ջ
			case '/':
			{
				postfix[dest]=0x20;
				dest++;

				while(1)
				{
					//��ջֱ�ӷ�
					if(top>=128)
					{
						top--;
						stack[top]=infix[source];
						break;
					}

					//�Ѿ���
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
			case '+':		//��%^,*/,+=����ȥ�������ջ
			case '-':
			{
				//printf("this=%c,stack[top]=%c\n",infix[source],stack[top]);
				postfix[dest]=0x20;
				dest++;

				while(1)
				{
					//��ջֱ�ӷ�
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

						postfix[dest]=stack[top];    //һֱ����ֱ�����������Ż��߿ո�
						dest++;

						top++;
					}
					else break;
				}

				top++;                    //�����������ӵ�
				break;
			}
			default:		//����ֱ�����
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
				break;  //���ո�˵�����ֽ���
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
				//���������
				stack2[top]=10*stack2[top]+(double)(postfix[num]-0x30);
			}
		}
		num++;
	}

	return stack2[0];
}