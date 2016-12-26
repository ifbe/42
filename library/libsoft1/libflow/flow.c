#define u64 unsigned long long
int motion_create(void*,void*);
int motion_delete();
int net_create(void*,void*);
int net_delete();
int sound_create(void*,void*);
int sound_delete();
int vision_create(void*,void*);
int vision_delete();




static int flow_list()
{
	return 0;
}
static int flow_choose()
{
	return 0;
}
static int flow_read()
{
	return 0;
}
static int flow_write()
{
	return 0;
}
static int flow_start(u64 type,char* p)
{
	return 0;
}
static int flow_stop()
{
	return 0;
}
int flow_create(char* world,u64* p)
{
	//
	p[0]=0;			//type
	p[1]=0x776f6c66;	//id

	p[10]=(u64)flow_start;
	p[11]=(u64)flow_stop;
	p[12]=(u64)flow_list;
	p[13]=(u64)flow_choose;
	p[14]=(u64)flow_read;
	p[15]=(u64)flow_write;

	//
	char* q=(char*)p+0x80;

	motion_create(world,q);
	q+=0x80;
	q += net_create(world,q);
	sound_create(world,q);
	q+=0x80;
	vision_create(world,q);
	q+=0x80;

	return q-(char*)p;
}
int flow_delete()
{
	vision_delete();
	sound_delete();
	return 0;
}
