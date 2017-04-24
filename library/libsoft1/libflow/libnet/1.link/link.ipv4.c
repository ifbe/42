#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct ipv4
{
	u8 dstmac[6];
	u8 srcmac[6];
	u8 type_86dd;

	u8 first;
	u8 second;
	u16 length;
	u16 identify;
	u16 flagfrag;
	u8 ttl;
	u8 proto;
	u16 checksum;
	u8 srcip[4];
	u8 dstip[4];
};




void ipv4_read()
{
}
void ipv4_write()
{
}
