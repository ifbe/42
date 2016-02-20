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
void file_list(char*);
void file_choose(char*);
void file_read(char*);
void file_write(char*);
void file_open(char*);
void file_close(char*);
//
void logic_list(char*);
void logic_choose(char*);
void logic_read(char*);
void logic_write(char*);
void logic_open(char*);
void logic_close(char*);
//
void real_list(char*);
void real_choose(char*);
void real_read(char*);
void real_write(char*);
void real_open(char*);
void real_close(char*);
//
void tcp_list(char*);
void tcp_choose(char*);
void tcp_read(char*);
void tcp_write(char*);
void tcp_open(char*);
void tcp_close(char*);
//
void udp_list(char*);
void udp_choose(char*);
void udp_read(char*);
void udp_write(char*);
void udp_open(char*);
void udp_close(char*);
//
void i2c_list(char*);
void i2c_choose(char*);
void i2c_read(char*);
void i2c_write(char*);
void i2c_open(char*);
void i2c_close(char*);
//
void usb_list(char*);
void usb_choose(char*);
void usb_read(char*);
void usb_write(char*);
void usb_open(char*);
void usb_close(char*);
//
int buf2arg(char*,char**,char**);
int compare(char*,char*);
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
	if(type==0x656c6966)real_list(p);
	if(type==0x6c616572)real_list(p);
	if(type==0x6369676f6c)logic_choose(p);
}
void final_choose(char* p)
{
	//if(type==what)what_choose();
	if(type==0x656c6966)real_choose(p);
	if(type==0x6c616572)real_choose(p);
	if(type==0x6369676f6c)logic_choose(p);
}
void final_read(char* p)
{
	//if(type==what)what_read();
	if(type==0x656c6966)real_read(p);
	if(type==0x6c616572)real_read(p);
	if(type==0x6369676f6c)logic_read(p);
}
void final_write(char* p)
{
	//if(type==what)what_write();
	if(type==0x656c6966)real_write(p);
	if(type==0x6c616572)real_write(p);
	if(type==0x6369676f6c)logic_write(p);
}
void final_open(char* p)
{
	int i=0;
	int ret=0;
	type=0;
	if(p==0)return;

	//如果是    xyz://    这种类型的，type=xyz
	while(1)
	{
		//quit?
		if(p[i]==0)break;

		//
		if( (p[i]==':')&&(p[i+1]=='/')&&(p[i+2]=='/') )
		{
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
	//	java://
	//	c://
	//	h://
	//	udp://
	//	tcp://
	//	http://
	//	sql://

	//3
	//	rgb://
	//	icon://

	//default
	if(type==0x656c6966)
	{
		file_open(p+ret);
	}
	else
	{
		type=0x6c616572;
		real_open(p);
	}
	//
}
void final_close(char* p)
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
		say("open ?            =init=mount=push=enter\n");
		say("close ?            =kill=unmount=pop=leave\n");

		return 2;
	}





        //4
        ret=compare( arg0 , "ls" );
        if(ret==0)
        {
                final_list(arg1);
                return 1;
        }
        ret=compare( arg0 , "cd" );
        if(ret==0)
        {
                final_choose(arg1);
                return 1;
        }
        ret=compare( arg0 , "read" );
        if(ret==0)
        {
                final_read(arg1);
                return 1;
        }
        ret=compare( arg0 , "write" );  //dangerous
        if(ret==0)
        {
                final_write(arg1);
                return 1;
        }




        //2
        ret=compare( arg0 , "open");
        if(ret==0)
        {
                final_open(arg1);
                return 1;
        }
        ret=compare( arg0 , "close");
        if(ret==0)
        {
                final_close(arg1);
                return 1;
        }

	return 2;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
