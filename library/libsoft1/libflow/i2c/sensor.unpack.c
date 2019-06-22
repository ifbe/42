#include "libsoft.h"




int unpack_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	say("@unpack_read\n");
	return 0;
}
int unpack_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct element* ele;
	struct relation* rel;
	say("@unpack_write\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	rel = ele->orel0;
	while(1){
		if(0 == rel)break;
		if(_art_ == rel->dsttype){
			arterywrite((void*)&rel->dstchip, (void*)&rel->srcchip, buf, len);
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
