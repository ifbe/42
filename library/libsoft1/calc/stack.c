#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




//------------------------------------------------




static double fpstack[20];
static int fpcount=0;
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




//-----------------------------------------------------




static QWORD stack[128];
static int sp=0;
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
