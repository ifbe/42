#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int motion_create(void*);
int motion_delete();
int netmgr_create(void*);
int netmgr_delete();
int sound_create(void*);
int sound_delete();
int vision_create(void*);
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
	motion_create(world);
	netmgr_create(world);
	sound_create(world);
	vision_create(world);
}
void flow_delete()
{
	vision_delete();
	sound_delete();
	netmgr_delete();
	motion_delete();
}
