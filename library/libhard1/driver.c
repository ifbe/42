#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void say(char*,...);




void driverread()
{
}
void driverwrite()
{
}
void driverstart()
{
}
void driverstop()
{
}
void driverlist(u8* buf)
{
	say("empth driver\n");
}
void driverchoose(u8* buf)
{
	say("@driver: %s\n", buf);
}
void drivercreate(char* addr)
{
	//say("[4,8):createed driver\n");
}
void driverdelete()
{
	//say("[4,8):deleteing driver\n");
}
