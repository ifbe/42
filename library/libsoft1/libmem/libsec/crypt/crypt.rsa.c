#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




void rsa2048(
	u8* dstbuf, int dstlen,
	u8* srcbuf, int srclen,
	u8* keybuf, int keylen,
	u8* modbuf, int modlen)
{
	//dst = (src^key)%mod
}
