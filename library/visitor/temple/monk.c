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
int file_list(char*);
int file_choose(char*);
int file_read(char*);
int file_write(char*);
int file_open(char*);
int file_close(char*);
//
int logic_list(char*);
int logic_choose(char*);
int logic_read(char*);
int logic_write(char*);
int logic_open(char*);
int logic_close(char*);
//
int tcp_list(char*);
int tcp_choose(char*);
int tcp_read(char*);
int tcp_write(char*);
int tcp_open(char*);
int tcp_close(char*);
//
int udp_list(char*);
int udp_choose(char*);
int udp_read(char*);
int udp_write(char*);
int udp_open(char*);
int udp_close(char*);
//
int i2c_list(char*);
int i2c_choose(char*);
int i2c_read(char*);
int i2c_write(char*);
int i2c_open(char*);
int i2c_close(char*);
//
int usb_list(char*);
int usb_choose(char*);
int usb_read(char*);
int usb_write(char*);
int usb_open(char*);
int usb_close(char*);
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
	if(type==0x656c6966)file_list(p);
	if(type==0x6369676f6c)logic_choose(p);
}
void final_choose(char* p)
{
	//if(type==what)what_choose();
	if(type==0x656c6966)file_choose(p);
	if(type==0x6369676f6c)logic_choose(p);
}
void final_read(char* p)
{
	//if(type==what)what_read();
	if(type==0x656c6966)file_read(p);
	if(type==0x6369676f6c)logic_read(p);
}
void final_write(char* p)
{
	//if(type==what)what_write();
	if(type==0x656c6966)file_write(p);
	if(type==0x6369676f6c)logic_write(p);
}
int final_open(char* p)
{
	int i=0;
	int ret=0;
	if(p==0)return -1;

	//如果是    xyz://    这种类型的，type=xyz
	type=0;
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

	//default
	say("type=%s\n",(char*)&type);
	if(type==0)type=0x656c6966;
	if(type==0x656c6966)
	{
		return file_open(p+ret);
	}

	//0
	//	acpi://
	else if(type==0x69706361)
	{
		//return acpi_open(p+ret);
	}
	//	dtb://
	else if(type==0x627464)
	{
		//return dtb_open(p+ret);
	}

	//1
	//	pci://
	if(type==0x696370)
	{
		//return pci_open(p+ret);
	}
	//	usb://
	else if(type==0x627375)
	{
		//return usb_open(p+ret);
	}
	//	i2c://
	else if(type==0x633269)
	{
		//return i2c_open(p+ret);
	}

	//2
	//	java://
	//	c://
	//	h://
	//	udp://
	else if(type==0x706475)
	{
		//return udp_open(p+ret);
	}
	//	tcp://
	else if(type==0x706374)
	{
		//return tcp_open(p+ret);
	}
	//	http://
	else if(type==0x70747468)
	{
		//return http_open(p+ret);
	}
	//	sql://
	else if(type==0x6c7173)
	{
		//return sql_open(p+ret);
	}

	//3
	//	rgb://
	else if(type==0x626772)
	{
		//return rgb_open(p+ret);
	}
	//	icon://
	else if(type==0x6e6f6369)
	{
		//return icon_open(p+ret);
	}

	return -1;
}
int final_close(char* p)
{
	return 0;
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
