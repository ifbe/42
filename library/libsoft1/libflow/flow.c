#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
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
	u8* q;

	p[0]=0;			//type
	p[1]=0x776f6c66;	//id
	p[2]=(u64)flow_start;
	p[3]=(u64)flow_stop;
	p[4]=(u64)flow_list;
	p[5]=(u64)flow_choose;
	p[6]=(u64)flow_read;
	p[7]=(u64)flow_write;
	q = (u8*)p;
	q += 0x100;

	motion_create(world, q);
	q+=0x100;

	net_create(world, q);
	q+=0x100;

	sound_create(world, q);
	q+=0x100;

	vision_create(world, q);
	q+=0x100;

	return q-(u8*)p;
}
int flow_delete()
{
	vision_delete();
	sound_delete();
	return 0;
}
