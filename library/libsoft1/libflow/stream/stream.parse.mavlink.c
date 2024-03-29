#include "libsoft.h"




struct mavlink{
	u8 ver;
	u8 len;
	u8 a;
	u8 b;

	u8 seq;
	u8 sid;
	u8 cid;

	u8 msg[3];
};
static inline void crc_accumulate(u8 data, u16* crcAccum)
{
	u8 tmp;

	tmp = data ^ (u8)(*crcAccum &0xff);
	tmp ^= (tmp<<4);
	*crcAccum = (*crcAccum>>8) ^ (tmp<<8) ^ (tmp <<3) ^ (tmp>>4);
}
static inline u16 crc_calculate(u8* pBuffer, int length)
{
	u16 crcTmp = 0xffff;
	while (length--) {
	crc_accumulate(*pBuffer++, &crcTmp);
	}
	return crcTmp;
}




int mavlinkclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int mavlinkclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int tmp;
	struct mavlink* ml = buf;
	printmemory(buf, len);

	tmp = ml->len;
	logtoall("ver = %d\n", 0xff - ml->ver);
	logtoall("len = %d\n", tmp);
	logtoall("flag = %d:%d\n", ml->a, ml->b);
	logtoall("seq = %d\n", ml->seq);
	logtoall("id = %d:%d\n", ml->sid, ml->cid);
	logtoall("msg = %d\n", ml->msg[0] | (ml->msg[1]<<8) | (ml->msg[2]<<16));
	printmemory(buf + 10, tmp);
	logtoall("crc=%x\n", crc_calculate(buf+1, 9+tmp));
	return 0;
}
int mavlinkclient_delete(_obj* ele)
{
	return 0;
}
int mavlinkclient_create(_obj* ele, u8* arg)
{
	logtoall("@mavlinkclient_create\n");
	return 0;
}





int mavlinkserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int mavlinkserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int mavlinkserver_delete(_obj* ele)
{
	return 0;
}
int mavlinkserver_create(_obj* ele, u8* arg)
{
	logtoall("@mavlinkserver_create\n");
	return 0;
}
