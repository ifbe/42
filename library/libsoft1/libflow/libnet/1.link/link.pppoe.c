#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long



struct pppoe
{
	u8 dstmac[6];
	u8 srcmac[6];
	u16 type_886x;		//x=3:discovery, x=4:session

	u8 version;
	u8 type;
	u16 code;
	u32 session;
	u32 length;
};




void pppoe_read()
{
}
void pppoe_write()
{
}
