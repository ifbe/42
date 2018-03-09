#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void say(char*,...);




int driverdelete()
{
	return 0;
}
int drivercreate()
{
	return 0;
}
int driverstop()
{
	return 0;
}
int driverstart()
{
	return 0;
}
int driverread()
{
	return 0;
}
int driverwrite()
{
	return 0;
}
int driverlist(u8* buf)
{
	say("empth driver\n");
	return 0;
}
int driverchoose(u8* buf)
{
	say("@driver: %s\n", buf);
	return 0;
}




void freedriver()
{
	//say("[4,8):freeing driver\n");
}
void initdriver(void* addr)
{
	//say("[4,8):inited driver\n");
}