#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




#define K 9.0e9
double coulomb(double q1,double q2,double r)
{
	return K*q1*q2/r/r;
}




void electrical_read()
{
}
void electrical_write()
{
}
void electrical_list()
{
}
void electrical_choose()
{
}
void electrical_start()
{
}
void electrical_stop()
{
}
void electrical_create(u8* softaddr, u64* p)
{
	//
	p[0]=0x73796870;
	p[1]=0x74697563726963;
	p[2]=(u64)electrical_start;
	p[3]=(u64)electrical_stop;
	p[4]=(u64)electrical_list;
	p[5]=(u64)electrical_choose;
	p[6]=(u64)electrical_read;
	p[7]=(u64)electrical_write;
}
void electrical_delete()
{
}
