#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//binary
int binary_list(char*);
int binary_choose(char*);
int binary_read(char*);
int binary_write(char*);
int binary_start(char*);
int binary_stop(char*);
int binary_init(char*);
int binary_kill();
//folder
int folder_list(char*);
int folder_switch(char*);
int folder_read(char*);
int folder_write(char*);
int folder_start(char*);
int folder_stop(char*);
int folder_init(char*);
int folder_kill();
//file system
int fs_list(char*);
int fs_choose(char*);
int fs_read(char*);
int fs_write(char*);
int fs_start(char*);
int fs_stop(char*);
int fs_init(char*);
int fs_kill();
//i2c
int i2c_list(char*);
int i2c_choose(char*);
int i2c_read(char*);
int i2c_write(char*);
int i2c_start(char*);
int i2c_stop(char*);
int i2c_init(char*);
int i2c_kill();
//partition table
int pt_list(char*);
int pt_choose(char*);
int pt_read(char*);
int pt_write(char*);
int pt_start(char*);
int pt_stop(char*);
int pt_init(char*);
int pt_kill();
//spi
int spi_list(char*);
int spi_choose(char*);
int spi_read(char*);
int spi_write(char*);
int spi_start(char*);
int spi_stop(char*);
int spi_init(char*);
int spi_kill();
//tcp
int tcp_list(char*);
int tcp_choose(char*);
int tcp_read(char*);
int tcp_write(char*);
int tcp_start(char*);
int tcp_stop(char*);
int tcp_init(char*);
int tcp_kill();
//udp
int udp_list(char*);
int udp_choose(char*);
int udp_read(char*);
int udp_write(char*);
int udp_start(char*);
int udp_stop(char*);
int udp_init(char*);
int udp_kill();
//usb
int usb_list(char*);
int usb_choose(char*);
int usb_read(char*);
int usb_write(char*);
int usb_start(char*);
int usb_stop(char*);
int usb_init(char*);
int usb_kill();
//
int buf2arg(BYTE* buf,int max,int* argc,BYTE** argv);
int buf2typename(BYTE* buf,int max,QWORD* type,BYTE** name);
void say(char*,...);




//binary-gpt4-ext4
//ether-ipv4-tcp-http-mp3......
static QWORD logictype[8]={0};
static int depth=0;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void arteryinit(char* module,char* addr)
{
	if(module==0)
	{
		binary_init(addr);		//1
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
int arterystart(BYTE* p)
{
	BYTE* name=0;
	int ret;

	ret=buf2typename(p,128,logictype,&name);
	if(ret==0)
	{
		say(	"@[%s]->[%s]->[%s]->[%s]\n",
			(char*)&logictype[0],
			(char*)&logictype[1],
			(char*)&logictype[2],
			(char*)&logictype[3]
		);
		return 0;		//fail1
	}

	if(logictype[0]==0)
	{
		//is this a folder?
		ret=folder_start(name);
		if(ret>0)
		{
			logictype[0]=0x7265646c6f66;
			return ret;
		}

		//is this a binary?
		ret=binary_start(name);
		if(ret>0)
		{
			//upgrade "type"???
			logictype[0]=0x7972616e6962;
			return ret;
		}

		//can't open
		say("open failed!\n");
		return 0;
	}

	//0
	//	acpi://
	else if(logictype[0]==0x69706361)
	{
		//return acpi_start(name);
	}
	//	dtb://
	else if(logictype[0]==0x627464)
	{
		//return dtb_start(name);
	}

	//1
	//	pci://
	else if(logictype[0]==0x696370)
	{
		//return pci_start(name);
	}
	//	usb://
	else if(logictype[0]==0x627375)
	{
		//return usb_start(name);
	}
	//	i2c://
	else if(logictype[0]==0x633269)
	{
		//return i2c_start(name);
	}

	//2
	//	java://
	//	c://
	//	h://
	//	udp://
	else if(logictype[0]==0x706475)
	{
		//return udp_start(name);
	}
	//	tcp://
	else if(logictype[0]==0x706374)
	{
		//return tcp_start(name);
	}
	//	http://
	else if(logictype[0]==0x70747468)
	{
		//return http_start(name);
	}
	//	sql://
	else if(logictype[0]==0x6c7173)
	{
		//return sql_start(name);
	}

	//3
	//	rgb://
	else if(logictype[0]==0x626772)
	{
		//return rgb_start(name);
	}
	//	icon://
	else if(logictype[0]==0x6e6f6369)
	{
		//return icon_start(name);
	}
	else say("unknown type\n");
}
int arterystop(char* p)
{
	if(logictype[0]==0)return 0;

	if(logictype[0]==0x7265646c6f66)
	{
		folder_stop(p);
		logictype[0]=0;
	}
	if(logictype[0]==0x7972616e6962)
	{
		binary_stop(p);
		logictype[0]=0;
	}
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void arterylist(char* p)
{
	//say("@arterylist\n");
	if(logictype[0]==0)return;

	if(logictype[0]==0x7265646c6f66)folder_list(p);
	if(logictype[0]==0x7972616e6962)binary_list(p);
	if(logictype[0]==0x6369676f6c)fs_list(p);
}
void arterychoose(char* p)
{
	//say("@arteryswitch\n");
	if(logictype[0]==0)return;

	if(logictype[0]==0x7265646c6f66)folder_switch(p);
	if(logictype[0]==0x7972616e6962)binary_choose(p);
	if(logictype[0]==0x6369676f6c)fs_choose(p);
}
void arteryread(char* p)
{
	//say("@arteryread\n");
	if(logictype[0]==0)return;

	if(logictype[0]==0x7265646c6f66)folder_read(p);
	if(logictype[0]==0x7972616e6962)binary_read(p);
	if(logictype[0]==0x6369676f6c)fs_read(p);
}
void arterywrite(char* p)
{
	//say("@arterywrite\n");
	if(logictype[0]==0)return;

	if(logictype[0]==0x7265646c6f66)folder_write(p);
	if(logictype[0]==0x7972616e6962)binary_write(p);
	if(logictype[0]==0x6369676f6c)fs_write(p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
