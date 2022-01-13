#include "libsoft.h"
#define _str_ hex32('s','t','r',0)
#define _vec_ hex32('v','e','c',0)
int parsefv(float* fbuf, int flen, u8* sbuf, int slen);




int str2vec_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int str2vec_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int j,cnt;
	float* f;
	float tmp_f[9];
	u8 tmp_u8[256];
	say("@str2vec_write: %.4s\n", &foot);

	switch(stack[sp-1].foottype){
	case _str_:
		//printmemory(buf, len);

		cnt = parsefv(tmp_f, 9, buf, len);
		return give_data_into_peer(art,_vec_, stack,sp, 0,0, tmp_f,cnt);
	case _vec_:
		cnt = 0;
		f = buf;
		for(j=0;j<len;j++){
			if(j == len-1)cnt += mysnprintf(tmp_u8+cnt, 256-cnt, "%f\n", f[j]);
			else cnt += mysnprintf(tmp_u8+cnt, 256-cnt, "%f, ", f[j]);
		}
		return give_data_into_peer(art,_str_, stack,sp, 0,0, tmp_u8,cnt);
	}
	return 0;
}
int str2vec_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int str2vec_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int str2vec_delete(_obj* art)
{
	return 0;
}
int str2vec_create(_obj* art, u8* url)
{
	say("@str2vec_create\n");
	return 1;
}
