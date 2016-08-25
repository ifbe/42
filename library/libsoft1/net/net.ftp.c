#define u64 unsigned long long




static void ftp_list()
{
}
static void ftp_choose()
{
}
static void ftp_read()
{
}
static void ftp_write()
{
}
static void ftp_start(u64 type,char* p)
{
}
static void ftp_stop()
{
}
void ftp_create(char* world,u64* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x707466;		//id
	p[2]=(u64)ftp_start;
	p[3]=(u64)ftp_stop;
	p[4]=(u64)ftp_list;
	p[5]=(u64)ftp_choose;
	p[6]=(u64)ftp_read;
	p[7]=(u64)ftp_write;
}
void ftp_delete()
{
	ftp_stop();
}
