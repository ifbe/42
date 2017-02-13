#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int bigpow(		//ans = (base^exp)%mod
        u8* base, int bl,
        u8* exp, int el,
        u8* mod, int ml,
        u8* ans, int al,
        u8* t1, int l1);




int rsa2048(		//dst = (src^key)%mod
	u8* dstbuf, int dstlen,
	u8* srcbuf, int srclen,
	u8* keybuf, int keylen,
	u8* modbuf, int modlen)
{
	u8 tmpbuf[0x200];
	return bigpow(
		srcbuf, srclen,
		keybuf, keylen,
		modbuf, modlen,
		dstbuf, dstlen,
		tmpbuf, 0x200
	);
}
