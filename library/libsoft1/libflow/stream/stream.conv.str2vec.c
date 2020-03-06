#include "libsoft.h"
#define _str_ hex32('s','t','r',0)
#define _vec_ hex32('v','e','c',0)
int parsefv(float* fbuf, int flen, u8* sbuf, int slen);




int str2vec_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int str2vec_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@str2vec_write:%.4s\n",&self->flag);
	struct artery* art = self->pchip;
	if(0 == art)return 0;

	if(_str_ == self->flag){
		int cnt;
		float tmp[9];
		struct artery* ele;
		//printmemory(buf, len);

		cnt = parsefv(tmp, 9, buf, len);
		return relationwrite(ele, _dst_, 0, 0, tmp, cnt);
	}
	if(_vec_ == self->flag){
		int j,cnt;
		float* f;
		u8 tmp[256];

		cnt = 0;
		f = buf;
		for(j=0;j<len;j++){
			if(j == len-1)cnt += mysnprintf(tmp+cnt, 256-cnt, "%f\n", f[j]);
			else cnt += mysnprintf(tmp+cnt, 256-cnt, "%f, ", f[j]);
		}
		return relationwrite(art, _dst_, 0, 0, tmp, cnt);
	}
	return 0;
}
int str2vec_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int str2vec_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int str2vec_delete(struct artery* art)
{
	return 0;
}
int str2vec_create(struct artery* art, u8* url)
{
	say("@str2vec_create\n");
	return 1;
}
