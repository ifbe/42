#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void say(char*,...);




int driverwrite_ev(void* ev)
{
	return 0;
}
int driverread_all()
{
	return 0;
}




int driverwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == dc)return driverwrite_ev(buf);
	return 0;
}
int driverread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	if(0 == sc)return driverread_all();
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
int driverdelete()
{
	return 0;
}
int drivercreate()
{
	return 0;
}
int driverchoose(u8* buf)
{
	say("@driver: %s\n", buf);
	return 0;
}
int driverlist(u8* buf)
{
	say("empth driver\n");
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