#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int bigpow(		//ans = (base^exp)%mod
        u8* ans, int al,
        u8* base, int bl,
        u8* exp, int el,
        u8* mod, int ml);
void printbigint(u8* p, int i);
void printmemory(u8* p, int i);
void logtoall(void*, ...);




void rsa2048(		//dst = (src^key)%mod
	u8* dstbuf, int dstlen,
	u8* srcbuf, int srclen,
	u8* keybuf, int keylen,
	u8* modbuf, int modlen)
{
	u8 tmp;
	int j,k;
	u8 bsebuf[0x1000];
	u8 expbuf[0x1000];
	for(j=0;j<srclen;j++)bsebuf[j] = srcbuf[srclen-1-j];
	for(j=srclen;j<0x800;j++)bsebuf[j] = 0;
	for(j=0;j<256;j++)expbuf[j] = keybuf[j];
	for(j=256;j<0x800;j++)expbuf[j] = 0;

logtoall("hex(pow(");
printbigint(bsebuf,srclen);
logtoall(",");
printbigint(expbuf,keylen);
logtoall(",");
printbigint(modbuf,modlen);
logtoall("))\n");
	k = bigpow(
		dstbuf, dstlen,
		bsebuf, srclen,
		expbuf, keylen,
		modbuf, modlen
	);

	for(j=0;j<k/2;j++)
	{
		tmp = dstbuf[j];
		dstbuf[j] = dstbuf[k-1-j];
		dstbuf[k-1-j] = tmp;
	}
logtoall("k=%d\n",k);
printbigint(dstbuf, k);
logtoall("\n");
}
