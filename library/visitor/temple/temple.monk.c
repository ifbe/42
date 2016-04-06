#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//event
int uievent(QWORD* first,QWORD* second);
int softevent(QWORD* first,QWORD* second);
int hardevent(QWORD* first,QWORD* second);
int bootevent(QWORD* first,QWORD* second);
//artery
void arteryinit(char*);
void arterykill(char*);
void arterystart(char*);
void arterystop(char*);
void arterylist(char*);
void arterychoose(char*);
void arteryread(char*);
void arterywrite(char*);
//
int buf2arg(BYTE* buf,int max,int* argc,BYTE** argv);
int buf2type(BYTE* buf,int max,QWORD* type,BYTE** name);
int compare(char*,char*);
void say(char*,...);




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int stillalive=1;
int waitevent(QWORD* first,QWORD* second)
{
        int ret;
        //say("here\n");
        if(stillalive==0)
        {
                first[0]=0;
                return 0;
        }

        //调试端口有没有消息
        ret=bootevent(first,second);
        if(ret>0)return 11;

        //硬件中断完成状态报告
        ret=hardevent(first,second);
        if(ret>0)return 22;

        //输入/网络/系统事件
        ret=softevent(first,second);
        if(ret>0)return 33;

        //窗口关闭,窗口大小变化等情况
        ret=uievent(first,second);
        if(ret>0)return 44;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int command(char* buffer)
{
	int ret;
	int argc;
	BYTE* argv[8];
	//say("command=%s\n",buffer);
	//printmemory(buffer,16);

	//convert
	buf2arg(buffer,128,&argc,argv);
	//say("argc=%x,argv@%llx\n",argc,argv);
	if(argc==0)return 0;
	if(argv[0]==0)return 0;

	//'q'
	if(argv[0][0]=='q')
	{
		stillalive=0;
		return 0;
	}
	//'exit
	ret=compare(argv[0],"exit");
	if(ret==0)
	{
		stillalive=0;
		return 0;
	}
	//'help'
	ret=compare(argv[0],"help");
	if(ret==0)
	{
		//"create","destory","start","stop"
		say("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		say("init ?             =create=make=fabricate\n");
		say("kill ?             =destory=smash=wreck\n");
		say("start ?            =open=mount=enter\n");
		say("stop ?             =close=unmount=leave\n");

		//"observe","change","get","put"
		say("ls ?               =list=summary=view=check\n");
		say("cd ?               =choose=into=switch=clap\n");
		say("read ?             =load=get=eat=copy\n");
		say("write ?            =store=put=spit=paste\n");
		say("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

		return 0;
	}

	//"create","destory","start","stop"
	ret=compare( argv[0] , "init");		//eg:	dynamicmodule
	if(ret==0)
	{
		arteryinit(argv[1]);
		return 2;
	}
	ret=compare( argv[0] , "kill");
	if(ret==0)
	{
		arterykill(argv[1]);
		return 2;
	}
	ret=compare( argv[0] , "start");
	if(ret==0)
	{
		arterystart(argv[1]);
		return 2;
	}
	ret=compare( argv[0] , "stop");
	if(ret==0)
	{
		arterystop(argv[1]);
		return 2;
	}

	//"observe","change","get","put"
	ret=compare( argv[0] , "ls" );
	if(ret==0)
	{
		arterylist(argv[1]);
		return 4;
	}
	ret=compare( argv[0] , "cd" );
	if(ret==0)
	{
		arterychoose(argv[1]);
		return 4;
	}
	ret=compare( argv[0] , "read" );
	if(ret==0)
	{
		arteryread(argv[1]);
		return 4;
	}
	ret=compare( argv[0] , "write" );  //dangerous
	if(ret==0)
	{
		arterywrite(argv[1]);
		return 4;
	}

	//
	return 8;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
