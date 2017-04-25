#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct icmp
{
	u8 type;	//8=request, 0=echo
	u8 code;
	u16 checksum;
	u16 identifer;
	u16 sequence;
	u8 timestamp[8];
};




void icmp_read()
{
}
void icmp_write()
{
}
