#define u64 unsigned long long




static void ir_list()
{
}
static void ir_choose()
{
}
static void ir_read()
{
}
static void ir_write()
{
}
static void ir_start(u64 type,char* p)
{
}
static void ir_stop()
{
}
void ir_create(char* world,u64* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x7269;		//id
	p[2]=(u64)ir_start;
	p[3]=(u64)ir_stop;
	p[4]=(u64)ir_list;
	p[5]=(u64)ir_choose;
	p[6]=(u64)ir_read;
	p[7]=(u64)ir_write;
}
void ir_delete()
{
	ir_stop();
}
