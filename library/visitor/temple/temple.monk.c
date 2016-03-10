#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//disk or file
int bin_list(char*);
int bin_choose(char*);
int bin_read(char*);
int bin_write(char*);
int bin_open(char*);
int bin_close(char*);
int bin_init();
int bin_kill();
//memory or mmio
int memory_list(char*);
int memory_choose(char*);
int memory_read(char*);
int memory_write(char*);
int memory_open(char*);
int memory_close(char*);
int memory_init();
int memory_kill();
//filesystem
int fs_list(char*);
int fs_choose(char*);
int fs_read(char*);
int fs_write(char*);
int fs_open(char*);
int fs_close(char*);
int fs_init();
int fs_kill();
//tcp
int tcp_list(char*);
int tcp_choose(char*);
int tcp_read(char*);
int tcp_write(char*);
int tcp_open(char*);
int tcp_close(char*);
int tcp_init();
int tcp_kill();
//udp
int udp_list(char*);
int udp_choose(char*);
int udp_read(char*);
int udp_write(char*);
int udp_open(char*);
int udp_close(char*);
int udp_init();
int udp_kill();
//i2c
int i2c_list(char*);
int i2c_choose(char*);
int i2c_read(char*);
int i2c_write(char*);
int i2c_open(char*);
int i2c_close(char*);
int i2c_init();
int i2c_kill();
//usb
int usb_list(char*);
int usb_choose(char*);
int usb_read(char*);
int usb_write(char*);
int usb_open(char*);
int usb_close(char*);
int usb_init();
int usb_kill();
//
int buf2arg(char*,char**,char**);
int compare(char*,char*);
void say(char*,...);








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static QWORD type=0;
void final_write(char* p)
{
	//if(type==what)what_write();
	if(type==0x656c6966)bin_write(p);
	//if(type==0x79726f6d656d)memory_write(p);
	if(type==0x6369676f6c)fs_write(p);
}
void final_read(char* p)
{
	//if(type==what)what_read();
	if(type==0x656c6966)bin_read(p);
	//if(type==0x79726f6d656d)memory_read(p);
	if(type==0x6369676f6c)fs_read(p);
}
void final_choose(char* p)
{
	//if(type==what)what_choose();
	if(type==0x656c6966)bin_choose(p);
	//if(type==0x79726f6d656d)memory_choose(p);
	if(type==0x6369676f6c)fs_choose(p);
}
void final_list(char* p)
{
	//if(type==what)what_list();
	if(type==0x656c6966)bin_list(p);
	//if(type==0x79726f6d656d)memory_list(p);
	if(type==0x6369676f6c)fs_list(p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
		return bin_open(p+ret);
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
	else if(type==0x79726f6d656d)
	{
		//return memory_open(p+ret);
	}
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
int final_init(char* p)
{
	//usb.module?
	//http.module?
	return 0;
}
int final_kill(char* p)
{
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
