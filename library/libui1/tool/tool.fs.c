#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
void background_start(QWORD size,void* addr);
void shape_start(QWORD size,void* addr);
void ascii_start(QWORD size,void* addr);
void unicode_start(QWORD size,void* addr);




static int width;
static int height;
static DWORD* screenbuf;




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
static void fs_start(QWORD size,void* addr)
{
	ascii_start(size,addr);
	unicode_start(size,addr);
	background_start(size,addr);
	shape_start(size,addr);

	//
	width=size&0xffff;
	height=(size>>16)&0xffff;
	screenbuf=addr;
}
static void fs_stop()
{
}
void fs_init(void* base,void* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x7366;
	this[2]=(QWORD)fs_start;
	this[3]=(QWORD)fs_stop;
	this[4]=(QWORD)fs_list;
	this[5]=(QWORD)fs_into;
	this[6]=(QWORD)fs_read;
	this[7]=(QWORD)fs_write;
}
void fs_kill()
{
}
