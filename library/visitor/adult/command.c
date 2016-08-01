#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//artery
void arteryinit(char*);
void arterykill(char*);
void arterystart(char*);
void arterystop(char*);
void arterylist(char*);
void arterychoose(char*);
void arteryread(char*);
void arterywrite(char*);
void writeevent();
//
int buf2arg(BYTE* buf,int max,int* argc,BYTE** argv);
int buf2type(BYTE* buf,int max,QWORD* type,BYTE** name);
int compare(char*,char*);
void say(char*,...);




static char* basic=0;
static char* body=0;
static char* memory=0;
static char* character=0;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int command(char* buffer)
{
	int ret;
	int argc;
	BYTE* argv[8];
	//say("command=%s\n",buffer);
	//printmemory(buffer,16);

	//"#"
	if(buffer[0]=='#')return 0;

	//convert
	buf2arg(buffer,128,&argc,argv);
	//say("argc=%x,argv@%llx\n",argc,argv);
	if(argc==0)return 0;

	//"enter key"
	if(argv[0]==0)goto finish;

	//q
	if(argv[0][0]=='q')
	{
		writeevent();
		return 0;
	}
	//exit
	ret=compare(argv[0],"exit");
	if(ret==0)
	{
		writeevent();
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

		goto finish;
	}

	//"create","destory","start","stop"
	ret=compare( argv[0] , "init");		//eg:	dynamicmodule
	if(ret==0)
	{
		arteryinit(argv[1]);
		goto finish;
	}
	ret=compare( argv[0] , "kill");
	if(ret==0)
	{
		arterykill(argv[1]);
		goto finish;
	}
	ret=compare( argv[0] , "start");
	if(ret==0)
	{
		arterystart(argv[1]);
		goto finish;
	}
	ret=compare( argv[0] , "stop");
	if(ret==0)
	{
		arterystop(argv[1]);
		goto finish;
	}

	//"observe","change","get","put"
	ret=compare( argv[0] , "ls" );
	if(ret==0)
	{
		arterylist(argv[1]);
		goto finish;
	}
	ret=compare( argv[0] , "cd" );
	if(ret==0)
	{
		arterychoose(argv[1]);
		goto finish;
	}
	ret=compare( argv[0] , "read" );
	if(ret==0)
	{
		arteryread(argv[1]);
		goto finish;
	}
	ret=compare( argv[0] , "write" );  //dangerous
	if(ret==0)
	{
		arterywrite(argv[1]);
		goto finish;
	}

finish:
	if(memory!=0)say("[%s]",memory);
	return 8;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void slaveinit(char* type,char* addr)
{
	basic=addr+0;
	body=addr+0x400000;
	memory=addr+0x800000;
	character=addr+0xc00000;
	say("[%s]",memory);
}
void slavekill()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
