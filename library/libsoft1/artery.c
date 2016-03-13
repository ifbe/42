#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//binary
int bin_list(char*);
int bin_choose(char*);
int bin_read(char*);
int bin_write(char*);
int bin_open(char*);
int bin_close(char*);
int bin_init(char*);
int bin_kill();
//file system
int fs_list(char*);
int fs_choose(char*);
int fs_read(char*);
int fs_write(char*);
int fs_open(char*);
int fs_close(char*);
int fs_init(char*);
int fs_kill();
//partition table
int pt_list(char*);
int pt_choose(char*);
int pt_read(char*);
int pt_write(char*);
int pt_open(char*);
int pt_close(char*);
int pt_init(char*);
int pt_kill();
//tcp
int tcp_list(char*);
int tcp_choose(char*);
int tcp_read(char*);
int tcp_write(char*);
int tcp_open(char*);
int tcp_close(char*);
int tcp_init(char*);
int tcp_kill();
//udp
int udp_list(char*);
int udp_choose(char*);
int udp_read(char*);
int udp_write(char*);
int udp_open(char*);
int udp_close(char*);
int udp_init(char*);
int udp_kill();
//i2c
int i2c_list(char*);
int i2c_choose(char*);
int i2c_read(char*);
int i2c_write(char*);
int i2c_open(char*);
int i2c_close(char*);
int i2c_init(char*);
int i2c_kill();
//usb
int usb_list(char*);
int usb_choose(char*);
int usb_read(char*);
int usb_write(char*);
int usb_open(char*);
int usb_close(char*);
int usb_init(char*);
int usb_kill();
//
int buf2arg(BYTE* buf,int max,int* argc,BYTE** argv);
int buf2type(BYTE* buf,int max,QWORD* type,BYTE** name);
void say(char*,...);








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static QWORD type=0;
static BYTE* name=0;
void arteryinit(char* type,char* addr)
{
	if(type==0)
	{
		bin_init(addr);	//1
        	fs_init(addr);		//2
	}
}
void arterykill()
{
}
int arteryopen(BYTE* p)
{
	//
	say("@arteryopen:%s\n",p);
	if(p==0)return 0;

	//"file://" "tcp://" ".c" ".asm"
	buf2type(p,128,&type,&name);
	if(type==0)
	{
		return bin_open(name);
	}

	//0
	//	acpi://
	else if(type==0x69706361)
	{
		//return acpi_open(name);
	}
	//	dtb://
	else if(type==0x627464)
	{
		//return dtb_open(name);
	}

	//1
	//	pci://
	if(type==0x696370)
	{
		//return pci_open(name);
	}
	//	usb://
	else if(type==0x627375)
	{
		//return usb_open(name);
	}
	//	i2c://
	else if(type==0x633269)
	{
		//return i2c_open(name);
	}

	//2
	//	java://
	//	c://
	//	h://
	//	udp://
	else if(type==0x706475)
	{
		//return udp_open(name);
	}
	//	tcp://
	else if(type==0x706374)
	{
		//return tcp_open(name);
	}
	//	http://
	else if(type==0x70747468)
	{
		//return http_open(name);
	}
	//	sql://
	else if(type==0x6c7173)
	{
		//return sql_open(name);
	}

	//3
	//	rgb://
	else if(type==0x626772)
	{
		//return rgb_open(name);
	}
	//	icon://
	else if(type==0x6e6f6369)
	{
		//return icon_open(name);
	}

	return -1;
}
int arteryclose(char* p)
{
	say("@arteryclose\n");
	if(type==0)bin_close(p);
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void arterylist(char* p)
{
	say("@arterylist\n");
	if(type==0)bin_choose(p);
	if(type==0)bin_list(p);
	if(type==0x6369676f6c)fs_list(p);
}
void arterychoose(char* p)
{
	say("@arteryswitch\n");
	if(type==0)bin_choose(p);
	if(type==0x6369676f6c)fs_choose(p);
}
void arteryread(char* p)
{
	say("@arteryread\n");
	if(type==0)bin_read(p);
	if(type==0x6369676f6c)fs_read(p);
}
void arterywrite(char* p)
{
	say("@arterywrite\n");
	if(type==0)bin_write(p);
	if(type==0x6369676f6c)fs_write(p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
