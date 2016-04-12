#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
QWORD prelibation(char*);
int compare(char*,char*);	//base tool
int hexstring2data(char*,QWORD*);
int mem2file(char* memaddr,char* filename,QWORD offset,QWORD count);
int file2mem(char* memaddr,char* filename,QWORD offset,QWORD count);
//
int systemstart(int,char*);
int systemstop();
int systemlist(char*);
int systemchoose(char*);
int systemread(char* rdi,QWORD rsi,QWORD rcx);
int systemwrite(char* rdi,QWORD rsi,QWORD rcx);
//
int printmemory(char* addr,int count);
int say(char* str,...);		//+1




//
static char* diskhome=0;
static char* fshome=0;
static char* dirhome=0;
static char* datahome=0;




//physical function
static int binary_list(char* arg1)
{
	QWORD temp=0;
	int i,j;
	char buf[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	QWORD target=0;
	char* addr=diskhome;
	//printmemory(diskhome,0x200);

	//想要什么
	if(arg1==0)target=0;
	else if( compare(arg1,"disk") == 0 )target=0x6b736964;	//'disk'
	else if( compare(arg1,"file") == 0 )
	{
		addr=dirhome;
		target=0;
		//target=0x656c6966;		//'file'
		//printmemory(addr,0x200);
	}
	else if( compare(arg1,"0") == 0 )
	{
		addr=diskhome;
		target=0;
	}
	else if( compare(arg1,"1") == 0 )
	{
		addr=fshome;
		target=0;
	}
	else if( compare(arg1,"2") == 0 )
	{
		addr=dirhome;
		target=0;
	}
	else if( compare(arg1,"3") == 0 )
	{
		addr=datahome;
		target=0;
	}

	//搜到就显示
	for(i=0; i<0x400; i++)		//0x40*0x400=0x10000
	{
		temp=*(QWORD*)( addr + (i*0x40) );
		if(temp == 0)break;
		if( (target==0) | (temp == target) )
		{
			//[+0]:type
			say("(%-4s," , addr+(i*0x40) );

			//[+8]:id
			*(QWORD*)buf=*(QWORD*)(addr+(i*0x40)+0x8);
			temp=0;
			for(j=0;j<8;j++)
			{
				//[1,0x1f]:wrong
				if(buf[j]>0 && buf[j]<0x20) temp++;

				//[0x80,0xff]:wrong
				if(buf[j]>=0x80) temp++;
			}
			if(temp==0) say("%4s)	",buf);
			else say("%4llx)	",*(QWORD*)buf);

			//[+10]:start
			say("[%-4llx,",*(QWORD*)(addr+(i*0x40)+0x10));

			//[+18]:end
			say("%4llx]	",*(QWORD*)(addr+(i*0x40)+0x18));

			//[+20]:detail
			say("{%-16s}	",addr+(i*0x40)+0x20);

			//which
			say("<%x>\n",i);
		}//if
	}//for

	return 1;
}//binary_list
static int binary_switch(char* arg)
{
}
static int binary_read(char* arg1)
{
	QWORD value;

	//nothing specified
	if(arg1==0)
	{
		say("read@%llx,world@%llx\n",binary_read,diskhome);
		return -1;
	}

	//what is it
	value=0;
	if( arg1[0] < '0' )value++;	//[0,0x2f]:wrong
	if( arg1[0] > 'f' )value++;	//[0x67,0xff]:wrong
	if( arg1[0] > '9' )		//[0x3a,0x60]:wrong
	{
		if( arg1[0] < 'a' )value++;
	}

	//default,read chosen memory/port/disk/socket...
	if(value==0)
	{
		hexstring2data(arg1,&value);
		systemread(datahome,value,1);
		printmemory(datahome,0x200);
		say("above is:%llx\n",value);
	}

	return 0;
}
static int binary_write(char* arg1)
{
	QWORD value;

	//nothing specified
	if(arg1==0)
	{
		say("write@%llx,world@%llx\n",binary_write,diskhome);
		return -1;
	}

	//what is it
	value=0;
	if( arg1[0] < '0' )value++;	//[0,0x2f]:wrong
	if( arg1[0] > 'f' )value++;	//[0x67,0xff]:wrong
	if( arg1[0] > '9' )		//[0x3a,0x60]:wrong
	{
		if( arg1[0] < 'a' )value++;
	}

	//"read memory.400000"
	if(value==0)
	{
		say("dangerous,bye\n");
	}

	return 0;
}








static int binary_start(QWORD type,char* p)
{
	int ret;

	//start
	ret=systemstart(1,p);
	if(ret<=0)return -1;

	//return
	return 1;
}
static int binary_stop()
{
	systemstop();
}
void binary_init(char* world,QWORD* p)
{
	//(自己)4块区域，每块1兆
	diskhome=world+0;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	//
	p[0]=0x6d656d;
	p[1]=0x6e6962;
	p[2]=(QWORD)binary_start;
	p[3]=(QWORD)binary_stop;
	p[4]=(QWORD)binary_list;
	p[5]=(QWORD)binary_switch;
	p[6]=(QWORD)binary_read;
	p[7]=(QWORD)binary_write;
}
void binary_kill()
{
	binary_stop();
}
