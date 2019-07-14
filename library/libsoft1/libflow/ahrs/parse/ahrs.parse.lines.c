#include "libsoft.h"
int parsefv(float* fbuf, int flen, u8* sbuf, int slen);




int unpack_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	say("@unpack_read\n");
	return 0;
}
int unpack_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	int cnt;
	float tmp[9];
	struct element* ele;
	struct relation* rel;
	say("@unpack_write:%.*s", len, buf);

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	cnt = parsefv(tmp, 9, buf, len);
	if( (cnt != 6) && (cnt != 9) ) {
		say("wrong fvcnt: %d\n", cnt);
		return 0;
	}

	rel = ele->orel0;
	while(1){
		if(0 == rel)break;
		if(_art_ == rel->dsttype){
			arterywrite((void*)&rel->dstchip, (void*)&rel->srcchip, tmp, cnt);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int unpack_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int unpack_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int unpack_create(struct element* ele, u8* url)
{
	say("@unpack_create\n");
	return 1;
}
