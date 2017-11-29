#define u8 unsigned char 
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int startsound(int rate, int chan);
int stopsound();
int readsound(void* buf, int len);
int writesound(void* buf, int len);
void printmemory(void*, int);
void say(void*, ...);




static int bb = 0;
int sound_explain(u64* p)
{
	if(bb != 0)say("sound@%x\n",p[0]);
	return 42;
}




//
void sound_list()
{
}
void sound_choose()
{
}
void sound_read()
{
}
void sound_write(char* buf, int len)
{
}
void sound_start()
{
	bb = 1;
	startsound(44100, 2);
}
void sound_stop()
{
	bb = 0;
	stopsound();
}
void sound_create()
{
}
void sound_delete()
{
}
