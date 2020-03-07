#include "libsoft.h"
u8 GXBUF[1] = {0x9};
u8 GYBUF[0x20] = {
0xd9, 0xd3, 0xce, 0x7e, 0xa2, 0xc5, 0xe9, 0x29,
0xb2, 0x61, 0x7c, 0x6d, 0x7e, 0x4d, 0x3d, 0x92,
0x4c, 0xd1, 0x48, 0x77, 0x2c, 0xdd, 0x1e, 0xe0,
0xb4, 0x86, 0xa0, 0xb8, 0xa1, 0x19, 0xae, 0x20};




//o(ox,oy) = draw tengent of a, find c=3rd intersect, find d=(c.x,-c.y)
int x25519_absame(
	u8* oxbuf, int oxlen,
	u8* oybuf, int oylen,
	u8* axbuf, int axlen,
	u8* aybuf, int aylen)
{
	return 0;
}




//o(ox,oy) = draw line (a,b), find c=3rd intersect, find d=(c.x,-c.y)
int x25519_abdiff(
	u8* oxbuf, int oxlen,
	u8* oybuf, int oylen,
	u8* axbuf, int axlen,
	u8* aybuf, int aylen,
	u8* bxbuf, int bxlen,
	u8* bybuf, int bylen)
{
	return 0;
}




//o = a^s = a+a+a+a+a+...
int x25519_multiply(
	u8* oxbuf, int oxlen,
	u8* oybuf, int oylen,
	u8* axbuf, int axlen,
	u8* aybuf, int aylen,
	u8* sbuf, int slen)
{
/*
	if(0 == s)return 0;

	while(1){
		if(1 == s)break;

		x25519_absame(
			oxbuf,oxlen, oybuf,oylen,
			axbuf,axlen, aybuf,aylen
		);
		s >> = 1;
	}
	x25519_abdiff(
		oxbuf,oxlen, oybuf,oylen,
		axbuf,axlen, aybuf,aylen,
		bxbuf,bxlen, bybuf,bylen
	);
*/
	return 0;
}




int x25519_pubkey(
	u8* oxbuf, int oxlen,
	u8* oybuf, int oylen,
	u8* sbuf, int slen)
{
	x25519_multiply(
		oxbuf,oxlen, oybuf,oylen,
		GXBUF,1, GYBUF,0x20,
		sbuf,slen);
	return 0;
}
