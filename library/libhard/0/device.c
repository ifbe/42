#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void say(char*,...);




int devicewrite_ev(void* ev)
{
	return 0;
}
int deviceread_all()
{
	return 0;
}




int devicewrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == dc)return devicewrite_ev(buf);
	return 0;
}
int deviceread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == sc)return deviceread_all();
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
int devicedelete()
{
	return 0;
}
int devicecreate()
{
	return 0;
}
int devicechoose(u8* buf)
{
	say("@device: %s\n", buf);
	return 0;
}
int devicelist(u8* buf)
{
	say("empth device\n");
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