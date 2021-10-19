#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct ipv6
{
	u8 dstmac[6];
	u8 srcmac[6];
	u8 type_0800;

	u32 first4;
	u16 length;
	u8 proto;
	u8 hop;

	u8 srcip6[16];
	u8 dstip6[16];
};




void ipv6_read()
{
}
void ipv6_write()
{
}
