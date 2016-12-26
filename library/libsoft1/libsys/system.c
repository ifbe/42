#define u64 unsigned long long
int folder_create(void*,void*);
int folder_delete();
int process_create(void*,void*);
int process_delete();




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
static int system_start(u64 type,char* p)
{
	return 0;
}
static int system_stop()
{
	return 0;
}
int system_create(char* world,u64* p)
{
	//
	p[0]=0;			//type
	p[1]=0x6d6574737973;	//id

	p[10]=(u64)system_start;
	p[11]=(u64)system_stop;
	p[12]=(u64)system_list;
	p[13]=(u64)system_choose;
	p[14]=(u64)system_read;
	p[15]=(u64)system_write;

	//
	char* q=(char*)p+0x80;

	folder_create(world,q);
	q+=0x80;
	process_create(world,q);
	q+=0x80;

	return q-(char*)p;
}
int system_delete()
{
	folder_delete();
	process_delete();
	return 0;
}
