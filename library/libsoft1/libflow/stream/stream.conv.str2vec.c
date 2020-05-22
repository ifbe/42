#include "libsoft.h"
#define _str_ hex32('s','t','r',0)
#define _vec_ hex32('v','e','c',0)
int parsefv(float* fbuf, int flen, u8* sbuf, int slen);




int str2vec_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int str2vec_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@str2vec_write: %.4s\n", &foot);
	if(_str_ == foot){
		int cnt;
		float tmp[9];
		//printmemory(buf, len);

		cnt = parsefv(tmp, 9, buf, len);
		return relationwrite(art,_vec_, stack,sp, 0,0, tmp,cnt);
	}
	if(_vec_ == foot){
		int j,cnt;
		float* f;
		u8 tmp[256];

		cnt = 0;
		f = buf;
		for(j=0;j<len;j++){
			if(j == len-1)cnt += mysnprintf(tmp+cnt, 256-cnt, "%f\n", f[j]);
			else cnt += mysnprintf(tmp+cnt, 256-cnt, "%f, ", f[j]);
		}
		return relationwrite(art,_str_, stack,sp, 0,0, tmp,cnt);
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
