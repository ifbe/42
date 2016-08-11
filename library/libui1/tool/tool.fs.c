#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char




static struct temp{
        QWORD type;
        QWORD id;
        QWORD start;
        QWORD end;

        QWORD pixelbuffer;
        QWORD pixelformat;
        QWORD width;
        QWORD height;
}*haha;




static void fs_list()
{
}
static void fs_into()
{
}
static void fs_read()
{
}
static void fs_write()
{
}
static void fs_start()
{
}
static void fs_stop()
{
}
void fs_init(void* base,void* addr)
{
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0x776f646e6977;
	this[1]=0x7366;

	this[10]=(QWORD)fs_start;
	this[11]=(QWORD)fs_stop;
	this[12]=(QWORD)fs_list;
	this[13]=(QWORD)fs_into;
	this[14]=(QWORD)fs_read;
	this[15]=(QWORD)fs_write;
}
void fs_kill()
{
}
