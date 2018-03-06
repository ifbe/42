#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void say(char*,...);




void deviceread()
{
}
void devicewrite()
{
}
void devicestart()
{
}
void devicestop()
{
}
void devicelist(u8* buf)
{
	say("empth device\n");
}
void devicechoose(u8* buf)
{
	say("@device: %s\n", buf);
}
void devicecreate(char* addr)
{
	//say("[4,8):createed device\n");
}
void devicedelete()
{
	//say("[4,8):deleteing device\n");
}
