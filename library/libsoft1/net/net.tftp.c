#define u64 unsigned long long




static void tftp_list()
{
}
static void tftp_choose()
{
}
static void tftp_read()
{
}
static void tftp_write()
{
}
static void tftp_start(u64 type,char* p)
{
}
static void tftp_stop()
{
}
void tftp_create(char* world,u64* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x70746674;	//id
	p[2]=(u64)tftp_start;
	p[3]=(u64)tftp_stop;
	p[4]=(u64)tftp_list;
	p[5]=(u64)tftp_choose;
	p[6]=(u64)tftp_read;
	p[7]=(u64)tftp_write;
}
void tftp_delete()
{
	tftp_stop();
}
