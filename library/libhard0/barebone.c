#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void initacpi();
void initpci();
void say(char*,...);




void drivercreate(char* type,char* addr)
{
	//say("[4,8):createed driver\n");
	initacpi();
	initpci();
}
void driverdelete()
{
	//say("[4,8):deleteing driver\n");
}
