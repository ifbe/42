#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




static void motion_list()
{
}
static void motion_choose()
{
}
static void motion_read()
{
}
static void motion_write()
{
}
static void motion_start()
{
}
static void motion_stop()
{
}
void motion_create(void* w, u64* p)
{
	p[0]=0x776f6c66;	//type
	p[1]=0x6e6f69746f6d;	//id
	p[2]=(u64)motion_start;
	p[3]=(u64)motion_stop;
	p[4]=(u64)motion_list;
	p[5]=(u64)motion_choose;
	p[6]=(u64)motion_read;
	p[7]=(u64)motion_write;
}
void motion_delete()
{
}
