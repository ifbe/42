#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void say(char*,...);




int devicedelete()
{
	return 0;
}
int devicecreate()
{
	return 0;
}
int devicestop()
{
	return 0;
}
int devicestart()
{
	return 0;
}
int deviceread()
{
	return 0;
}
int devicewrite()
{
	return 0;
}
int devicelist(u8* buf)
{
	say("empth device\n");
	return 0;
}
int devicechoose(u8* buf)
{
	say("@device: %s\n", buf);
	return 0;
}




void freedevice()
{
	//say("[4,8):freeing device\n");
}
void initdevice(void* addr)
{
	//say("[4,8):inited device\n");
}