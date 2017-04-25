#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct udp
{
	u16 srcport;
	u16 dstport;
	u16 length;
	u16 checksum;
};




void udp_read()
{
}
void udp_write()
{
}
