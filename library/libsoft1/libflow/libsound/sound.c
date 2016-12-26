#define u8 unsigned char 
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int startsound(int rate, int chan);
int stopsound();
int readsound(void* buf, int len);
int writesound(void* buf, int len);




//
static u8* datahome;




static void sound_list()
{
}
static void sound_choose()
{
	while(1)
	{
		readsound(datahome, 1024);
		writesound(datahome, 1024);
	}
}
static void sound_read()
{
}
static void sound_write()
{
}
static void sound_start()
{
	startsound(44100, 2);
}
static void sound_stop()
{
	stopsound();
}
void sound_create(void* w, u64* p)
{
	p[0]=0x776f6c66;	//type
	p[1]=0x646e756f73;	//id

	p[10]=(u64)sound_start;
	p[11]=(u64)sound_stop;
	p[12]=(u64)sound_list;
	p[13]=(u64)sound_choose;
	p[14]=(u64)sound_read;
	p[15]=(u64)sound_write;

	datahome = w + 0x300000;
}
void sound_delete()
{
}




void sound_explain(u64* p)
{
}
