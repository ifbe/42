#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




void swap_u8(u8* p, u8* q)
{
	if(p == 0)return;
	if(q == 0)return;

	u8 temp = p[0];
	p[0] = q[0];
	q[0] = temp;
}
void swap_u16(u16* p, u16* q)
{
	if(p == 0)return;
	if(q == 0)return;

	u16 temp = p[0];
	p[0] = q[0];
	q[0] = temp;
}
void swap_u32(u32* p, u32* q)
{
	if(p == 0)return;
	if(q == 0)return;

	u32 temp = p[0];
	p[0] = q[0];
	q[0] = temp;
}
void swap_u64(u64* p, u64* q)
{
	if(p == 0)return;
	if(q == 0)return;

	u64 temp = p[0];
	p[0] = q[0];
	q[0] = temp;
}




void swap_endian(u8* buf, int len)
{
	int j;
	u8 temp;
	if(buf == 0)return;
	if((len&1) == 1)return;

	for(j=0;j<len/2;j++)
	{
		temp = buf[j];
		buf[j] = buf[len-1-j];
		buf[len-1-j] = temp;
	}
}
