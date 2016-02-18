#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
int uievent(QWORD* first,QWORD* second);
int softevent(QWORD* first,QWORD* second);
int hardevent(QWORD* first,QWORD* second);
int bootevent(QWORD* first,QWORD* second);
//
void say(char*,...);








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int stillalive=1;
int waitevent(QWORD* first,QWORD* second)
{
	int ret;
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static QWORD type=0;
void final_list(char* p)
{
	//if(type==what)what_list();
	if(type==0)real_list();
	if(type==0x6c616572)real_list();
	if(type==0x6369676f6c)logic_ls();
}
void final_choose(char* p)
{
	//if(type==what)what_choose();
	if(type==0)real_choose();
	if(type==0x6c616572)real_choose();
	if(type==0x6369676f6c)logic_cd();
}
void final_read(char* p)
{
	//if(type==what)what_read();
	if(type==0){say("which?");return;}
	if(type==0x6c616572)real_read();
	if(type==0x6369676f6c)logic_read();
}
void final_write(char* p)
{
	//if(type==what)what_write();
	if(type==0){say("which?\n");return;}
	if(type==0x6c616572)real_write();
	if(type==0x6369676f6c)logic_write();
}
void enter(char* p)
{
	int i=0;
	int ret=0;

	//如果是    xyz://    这种类型的，type=xyz
	if(p==0)return;
	while(1)
	{
		//quit?
		if(p[i]==0)break;

		//
		if( (p[i]==':')&&(p[i+1]=='/')&&(p[i+2]=='/') )
		{
			type=0;
			for(ret=i-1;ret>=0;ret--)
			{
				type = type<<8;
				type+=p[ret];
			}
			ret=i+3;
			break;
		}

		//
		i++;
	}

	say("%s\n",(char*)&type);
	//0
	//	acpi://
	//	dtb://

	//1
	//	pci://
	//	usb://
	//	i2c://

	//2
	//	file://
	if(type==0x656c6966)
	{
		real_choose(p+ret);
	}
	//	java://
	//	c://
	//	h://
	//	udp://
	//	tcp://
	//	http://
	//	sql://

	//3
	//	image://

	//default
	//
}
void leave(char* p)
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int command(char* buffer)
{
	int ret;
	char* arg0;
	char* arg1;
	say("%s\n",buffer);
	buf2arg(buffer,&arg0,&arg1);
	if(arg0==0)return 0;




	//'q'
	if(arg0[0]=='q')
	{
		stillalive=0;
		return 1;
	}
	//'exit
	if((arg0[0]==0x65)&&(arg0[1]==0x78)&&(arg0[2]==0x69)&&(arg0[3]==0x74))
	{
		stillalive=0;
		return 1;
	}
	//'help'
	if((arg0[0]==0x68)&&(arg0[1]==0x65)&&(arg0[2]==0x6c)&&(arg0[3]==0x70))
	{
		//4
		say("ls ?               =list=summary=view=check\n");
		say("cd ?               =choose=into=switch=clap\n");
		say("read ?             =load=get=eat=copy\n");
		say("write ?            =store=put=spit=paste\n\n");

		//2
		say("enter ?            =init=mount=push=open\n");
		say("leave ?            =kill=unmount=pop=close\n");

		return 2;
	}





        //4
        ret=compare( arg0 , "ls" );
        if(ret==0)
        {
                real_list(arg1);
                return 1;
        }
        ret=compare( arg0 , "cd" );
        if(ret==0)
        {
                real_choose(arg1);
                return 1;
        }
        ret=compare( arg0 , "read" );
        if(ret==0)
        {
                real_read(arg1);
                return 1;
        }
        ret=compare( arg0 , "write" );  //dangerous
        if(ret==0)
        {
                real_write(arg1);
                return 1;
        }




        //2
        ret=compare( arg0 , "enter");
        if(ret==0)
        {
                enter(arg1);
                return 1;
        }
        ret=compare( arg0 , "leave");
        if(ret==0)
        {
                leave(arg1);
                return 1;
        }

	return 2;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
