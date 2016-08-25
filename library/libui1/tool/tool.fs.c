#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char




static struct temp{
        u64 type;
        u64 id;
        u64 start;
        u64 end;

        u64 pixelbuffer;
        u64 pixelformat;
        u64 width;
        u64 height;
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
static void fs_write(u64* type,u64* key)
{
}
static void fs_start()
{
}
static void fs_stop()
{
}
void fs_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x6c6f6f74;
	this[1] = 0x7366;

	this[10]=(u64)fs_start;
	this[11]=(u64)fs_stop;
	this[12]=(u64)fs_list;
	this[13]=(u64)fs_into;
	this[14]=(u64)fs_read;
	this[15]=(u64)fs_write;
}
void fs_delete()
{
}
