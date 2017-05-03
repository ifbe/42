#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int folder_create(void*,void*);
int folder_delete();
int monitor_create(void*,void*);
int monitor_delete();
int shell_create(void*,void*);
int shell_delete();




static int system_list()
{
	return 0;
}
static int system_choose()
{
	return 0;
}
static int system_read()
{
	return 0;
}
static int system_write()
{
	return 0;
}
static int system_start()
{
	return 0;
}
static int system_stop()
{
	return 0;
}
int system_create(char* world,u64* p)
{
	u8* q;

	//
	p[0]=0;			//type
	p[1]=0x6d6574737973;	//id
	p[2]=(u64)system_start;
	p[3]=(u64)system_stop;
	p[4]=(u64)system_list;
	p[5]=(u64)system_choose;
	p[6]=(u64)system_read;
	p[7]=(u64)system_write;

	//
	q=(u8*)p;
	q += 0x100;

	folder_create(world,q);
	q += 0x100;

	monitor_create(world,q);
	q += 0x100;

	shell_create(world,q);
	q += 0x100;

	return q-(u8*)p;
}
int system_delete()
{
	folder_delete();
	monitor_delete();
	shell_delete();
	return 0;
}
