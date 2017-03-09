#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define G 6.67259e-11




double gravity(double m1,double m2,double r)
{
	return G*m1*m1/r/r;
}




void mechanical_read()
{
}
void mechanical_write()
{
}
void mechanical_list()
{
}
void mechanical_choose()
{
}
void mechanical_start()
{
}
void mechanical_stop()
{
}
void mechanical_create(u8* softaddr, u64* p)
{
	//
	p[0]=0x73796870;
	p[1]=0x6863656d;
	p[2]=(u64)mechanical_start;
	p[3]=(u64)mechanical_stop;
	p[4]=(u64)mechanical_list;
	p[5]=(u64)mechanical_choose;
	p[6]=(u64)mechanical_read;
	p[7]=(u64)mechanical_write;
}
void mechanical_delete()
{
}
