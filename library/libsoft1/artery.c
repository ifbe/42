#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//binary
int bin_init(char*);
int bin_kill();
//file system
int fs_init(char*);
int fs_kill();
//partition table
int pt_init(char*);
int pt_kill();
//tcp
int tcp_init(char*);
int tcp_kill();
//udp
int udp_init(char*);
int udp_kill();
//i2c
int i2c_init(char*);
int i2c_kill();
//usb
int usb_init(char*);
int usb_kill();
//
void say(char*,...);








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void initartery(char* world)
{
	bin_init(world);	//1
        fs_init(world);		//2
}
void killartery()
{
}
void openartery(char* what)
{
	//选一个操作者
}
void closeartery()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
