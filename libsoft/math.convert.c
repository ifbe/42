#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




static char stack[128];
static int count=0;




//��ջ��ʱ��rspֵ���:		rsp=0xa0000		(count=128)
//��ջ��ʱ��rspֵ��С:		rsp=0x90000		(count=0)
static void initstack()
{
	count=128;
}

//push rax:
//rsp-8,[rsp]=rax		(count--,stack[count]=data)
static int push(char data)
{
	//��ջ
	if(count==0)return 0;

	//count-1(rsp-8)��Ȼ������������
	count--;
	stack[count]=data;
	return 1;
}

//pop rax:
//rax=[rsp],rsp+8		(data=stack[count],count++)
static int pop(char* dest)
{
	//��ջ
	if(count>=128)return 0;

	//�ó���ǰ���֣�Ȼ��count+1(rsp+8)
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
			case 0:		//�����ˣ���ջ���ܰ��ȫ���ȥ����
			{
				while(1)
				{
					ret=pop(postfix+dest);
					if(ret<=0)return;

					dest++;
				}
			}
			case '(':
			case '[':		//��������������ջ
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
					//��һ������
					ret=pop(&stacktop);

					//��������
					if( ret<=0 )return;

					//�������ŵĻ�ֱ���ӵ�����������žʹ�������
					if( stacktop == '(' )break;

					//����Ѹսг����ķ��ߣ���������һ��������
					else
					{
						postfix[dest]=stacktop;    //һֱ����ֱ�����������Ż��߿ո�
						dest++;
					}
				}

				break;
			}//������
			case '+':		//��%,^,/,*����ȥ�������ջ
			case '-':
			case '*':
			case '/':
			case '^':		//�˷�
			case '%':		//ȡ��
			case '!':		//�׳�
			{
				postfix[dest]=0x20;
				dest++;

				while(1)
				{
					//��һ���������Լ���
					ret=pop(&stacktop);
					compareresult=operatorpriority(stacktop) - operatorpriority(infix[source]);

					//��ջֱ�ӷţ����Լ�������������ջ������Լ�Ҳ��ջ����
					if( (ret<=0) | (compareresult<0) )
					{
						push( stacktop );
						push( infix[source] );
						break;
					}

					//���Լ�һ���������߱��Լ�ǿ������������Ȼ���Լ������´�ѭ��
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
			default:		//����ֱ�����
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