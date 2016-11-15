#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




//
static u8* datahome;




static void websocket_list()
{
}
static void websocket_choose()
{
}
static void websocket_read()
{
}
static void websocket_write()
{
}
static void websocket_start(u64 type,char* p)
{
}
static void websocket_stop()
{
}
void websocket_create(char* softhome, u64* p)
{
	//
	datahome = softhome + 0x300000;

/*
	p[0]=0x74656e;		//type
	p[1]=0x7377;		//id

	p[10]=(u64)websocket_start;
	p[11]=(u64)websocket_stop;
	p[12]=(u64)websocket_list;
	p[13]=(u64)websocket_choose;
	p[14]=(u64)websocket_read;
	p[15]=(u64)websocket_write;
*/
}
void websocket_delete()
{
	websocket_stop();
}
