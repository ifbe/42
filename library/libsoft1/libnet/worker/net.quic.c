#define u64 unsigned long long




static void quic_list()
{
}
static void quic_choose()
{
}
static void quic_read()
{
}
static void quic_write()
{
}
static void quic_start(u64 type,char* p)
{
}
static void quic_stop()
{
}
void quic_create(char* world,u64* p)
{
/*
	p[0]=0x74656e;		//type
	p[1]=0x63697571;	//id

	p[10]=(u64)quic_start;
	p[11]=(u64)quic_stop;
	p[12]=(u64)quic_list;
	p[13]=(u64)quic_choose;
	p[14]=(u64)quic_read;
	p[15]=(u64)quic_write;
*/
}
void quic_delete()
{
	quic_stop();
}
