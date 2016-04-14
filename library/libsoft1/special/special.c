#define QWORD unsigned long long
int folder_init(void*,void*);
int folder_kill();




static int special_list()
{
}
static int special_switch()
{
}
static int special_read()
{
}
static int special_write()
{
}
static int special_start(QWORD type,char* p)
{
}
static int special_stop()
{
}
int special_init(char* world,QWORD* p)
{
	//
	p[0]=0;			//type
	p[1]=0x6c616963657073;	//id
	p[2]=(QWORD)special_start;
	p[3]=(QWORD)special_stop;
	p[4]=(QWORD)special_list;
	p[5]=(QWORD)special_switch;
	p[6]=(QWORD)special_read;
	p[7]=(QWORD)special_write;

	//
	char* q=(char*)p+0x40;

	folder_init(world,q);
	q+=0x40;

	return q-(char*)p;
}
int special_kill()
{
	folder_kill();
}
