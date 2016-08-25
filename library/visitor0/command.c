#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//artery
void arterycreate(char*);
void arterydelete(char*);
void arterystart(char*);
void arterystop(char*);
void arterylist(char*);
void arterychoose(char*);
void arteryread(char*);
void arterywrite(char*);
//
void eventread(u32,u32);
void eventwrite();
//
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2type(u8* buf,int max,u64* type,u8** name);
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
	u8* argv[8];
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
		eventwrite();
		return 0;
	}
	//exit
	ret=compare(argv[0],"exit");
	if(ret==0)
	{
		eventwrite();
		return 0;
	}

	//'help'
	ret=compare(argv[0],"help");
	if(ret==0)
	{
		//"create","destory","start","stop"
		say("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		say("create ?             =create=make=fabricate\n");
		say("delete ?             =destory=smash=wreck\n");
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
	ret=compare( argv[0] , "create");		//eg:	dynamicmodule
	if(ret==0)
	{
		arterycreate(argv[1]);
		goto finish;
	}
	ret=compare( argv[0] , "delete");
	if(ret==0)
	{
		arterydelete(argv[1]);
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
void slavecreate(char* type,char* addr)
{
	basic=addr+0;
	body=addr+0x400000;
	memory=addr+0x800000;
	character=addr+0xc00000;
	say("[%s]",memory);
}
void slavedelete()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
