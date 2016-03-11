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
int bin_init();
int bin_kill();
//default
int default_list(char*);
int default_choose(char*);
int default_read(char*);
int default_write(char*);
int default_open(char*);
int default_close(char*);
int default_init();
int default_kill();
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
int buf2arg(BYTE* buf,int max,int* argc,BYTE** argv);
int buf2type(BYTE* buf,int max,QWORD* type,BYTE** name);
void say(char*,...);








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static QWORD type=0;
void final_write(char* p)
{
	//if(type==what)what_write();
	if(type==0)default_write(p);
	if(type==0x6369676f6c)fs_write(p);
}
void final_read(char* p)
{
	//if(type==what)what_read();
	if(type==0)default_read(p);
	if(type==0x6369676f6c)fs_read(p);
}
void final_choose(char* p)
{
	//if(type==what)what_choose();
	if(type==0)default_choose(p);
	if(type==0x6369676f6c)fs_choose(p);
}
void final_list(char* p)
{
	//if(type==what)what_list();
	if(type==0)default_list(p);
	if(type==0x6369676f6c)fs_list(p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int final_open(BYTE* p)
{
	//"file://" "tcp://" ".c" ".asm"
	QWORD type;
	BYTE* name;
	buf2type(p,128,&type,&name);
	if(type==0)
	{
		return default_open(name);
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
int final_close(char* p)
{
	if(type==0)default_close(p);
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
