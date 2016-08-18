#define QWORD unsigned long long




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
static void ftp_start(QWORD type,char* p)
{
}
static void ftp_stop()
{
}
void ftp_create(char* world,QWORD* p)
{
	//
	p[0]=0x74656e;		//type
	p[1]=0x707466;		//id
	p[2]=(QWORD)ftp_start;
	p[3]=(QWORD)ftp_stop;
	p[4]=(QWORD)ftp_list;
	p[5]=(QWORD)ftp_choose;
	p[6]=(QWORD)ftp_read;
	p[7]=(QWORD)ftp_write;
}
void ftp_delete()
{
	ftp_stop();
}
