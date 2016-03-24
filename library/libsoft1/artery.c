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
//folder
int folder_list(char*);
int folder_switch(char*);
int folder_read(char*);
int folder_write(char*);
int folder_open(char*);
int folder_close(char*);
int folder_init(char*);
int folder_kill();
//file system
int fs_list(char*);
int fs_choose(char*);
int fs_read(char*);
int fs_write(char*);
int fs_open(char*);
int fs_close(char*);
int fs_init(char*);
int fs_kill();
//i2c
int i2c_list(char*);
int i2c_choose(char*);
int i2c_read(char*);
int i2c_write(char*);
int i2c_open(char*);
int i2c_close(char*);
int i2c_init(char*);
int i2c_kill();
//partition table
int pt_list(char*);
int pt_choose(char*);
int pt_read(char*);
int pt_write(char*);
int pt_open(char*);
int pt_close(char*);
int pt_init(char*);
int pt_kill();
//spi
int spi_list(char*);
int spi_choose(char*);
int spi_read(char*);
int spi_write(char*);
int spi_open(char*);
int spi_close(char*);
int spi_init(char*);
int spi_kill();
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
int buf2typename(BYTE* buf,int max,QWORD* type,BYTE** name);
void say(char*,...);




//
static QWORD uppertype=0;
static BYTE* name=0;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void arteryinit(char* module,char* addr)
{
	if(module==0)
	{
		bin_init(addr);		//1
		fs_init(addr);		//2
		pt_init(addr);		//3
	}
/*
	else
	{
		initmodule(module);
	}
*/
}
void arterykill(char* module)
{
	//killmodule(module);
}
int arteryopen(BYTE* p)
{
	int ret=buf2typename(p,128,&uppertype,&name);
	if(ret==0)
	{
		say("null pointer\n");
		return 0;		//fail1
	}

	if(uppertype==0)
	{
		//is this a folder?
		ret=folder_open(name);
		if(ret>0)
		{
			uppertype=0;
			return ret;
		}

		//is this a binary?
		ret=bin_open(name);
		if(ret>0)
		{
			//upgrade "type"???
			uppertype=1;
			return ret;
		}

		//can't open
		say("open failed!\n");
		return 0;
	}

	//0
	//	acpi://
	else if(uppertype==0x69706361)
	{
		//return acpi_open(name);
	}
	//	dtb://
	else if(uppertype==0x627464)
	{
		//return dtb_open(name);
	}

	//1
	//	pci://
	if(uppertype==0x696370)
	{
		//return pci_open(name);
	}
	//	usb://
	else if(uppertype==0x627375)
	{
		//return usb_open(name);
	}
	//	i2c://
	else if(uppertype==0x633269)
	{
		//return i2c_open(name);
	}

	//2
	//	java://
	//	c://
	//	h://
	//	udp://
	else if(uppertype==0x706475)
	{
		//return udp_open(name);
	}
	//	tcp://
	else if(uppertype==0x706374)
	{
		//return tcp_open(name);
	}
	//	http://
	else if(uppertype==0x70747468)
	{
		//return http_open(name);
	}
	//	sql://
	else if(uppertype==0x6c7173)
	{
		//return sql_open(name);
	}

	//3
	//	rgb://
	else if(uppertype==0x626772)
	{
		//return rgb_open(name);
	}
	//	icon://
	else if(uppertype==0x6e6f6369)
	{
		//return icon_open(name);
	}
	else say("unknown type\n");
}
int arteryclose(char* p)
{
	//say("@arteryclose\n");
	if(uppertype==0)folder_close(p);
	if(uppertype==1)bin_close(p);
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void arterylist(char* p)
{
	//say("@arterylist\n");
	if(uppertype==0)folder_list(p);
	if(uppertype==1)bin_list(p);
	if(uppertype==0x6369676f6c)fs_list(p);
}
void arterychoose(char* p)
{
	//say("@arteryswitch\n");
	if(uppertype==0)folder_switch(p);
	if(uppertype==1)bin_choose(p);
	if(uppertype==0x6369676f6c)fs_choose(p);
}
void arteryread(char* p)
{
	//say("@arteryread\n");
	if(uppertype==0)folder_read(p);
	if(uppertype==1)bin_read(p);
	if(uppertype==0x6369676f6c)fs_read(p);
}
void arterywrite(char* p)
{
	//say("@arterywrite\n");
	if(uppertype==0)folder_write(p);
	if(uppertype==1)bin_write(p);
	if(uppertype==0x6369676f6c)fs_write(p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
