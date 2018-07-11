#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct arp
{
	u8 dstmac[6];
	u8 srcmac[6];
	u16 type_0806;

	u16 hwtype;
	u16 prototype;
	u8 hwsize;
	u8 protosize;
	u16 opcode;
	u8 sendermac[6];
	u8 senderip[4];
	u8 targetmac[6];
	u8 targetip[4];
	u8 zero[18];
};




void arp_read()
{
}
void arp_write()
{
}
