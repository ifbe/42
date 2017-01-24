#define u64 unsigned long long
//
void startvision();
void stopvision();
//
void printmemory(void*, int);
void say(void*, ...);




void vision_explain(u64* p)
{
	say("%llx\n",p[0]);
}




static void vision_list()
{
}
static void vision_choose()
{
}
static void vision_read()
{
}
static void vision_write()
{
}
static void vision_start()
{
	startvision();
}
static void vision_stop()
{
	stopvision();
}
void vision_create(void* w, u64* p)
{
	p[0]=0x776f6c66;	//type
	p[1]=0x6e6f69736976;	//id

	p[10]=(u64)vision_start;
	p[11]=(u64)vision_stop;
	p[12]=(u64)vision_list;
	p[13]=(u64)vision_choose;
	p[14]=(u64)vision_read;
	p[15]=(u64)vision_write;
}
void vision_delete()
{
}

