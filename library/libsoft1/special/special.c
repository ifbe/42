#define u64 unsigned long long
int folder_create(void*,void*);
int folder_delete();
int process_create(void*,void*);
int process_delete();




static int special_list()
{
}
static int special_choose()
{
}
static int special_read()
{
}
static int special_write()
{
}
static int special_start(u64 type,char* p)
{
}
static int special_stop()
{
}
int special_create(char* world,u64* p)
{
	//
	p[0]=0;			//type
	p[1]=0x6c616963657073;	//id

	p[10]=(u64)special_start;
	p[11]=(u64)special_stop;
	p[12]=(u64)special_list;
	p[13]=(u64)special_choose;
	p[14]=(u64)special_read;
	p[15]=(u64)special_write;

	//
	char* q=(char*)p+0x80;

	folder_create(world,q);
	q+=0x80;
	process_create(world,q);
	q+=0x80;

	return q-(char*)p;
}
int special_delete()
{
	folder_delete();
	process_delete();
}
