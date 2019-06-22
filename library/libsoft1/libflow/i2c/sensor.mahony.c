#include "libsoft.h"




int mahony_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct element* ele;
	struct relation* rel;
	say("@mahony_read\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	rel = ele->irel0;
	while(1){
		if(0 == rel)break;
		if(_art_ == rel->srctype){
			arteryread((void*)&rel->srcchip, (void*)&rel->dstchip, buf, len);
		}
		rel = samedstnextsrc(rel);
	}

	return 0;
}
int mahony_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct element* ele;
	struct relation* rel;
	say("@mahony_write\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	rel = ele->orel0;
	while(1){
		if(0 == rel)break;
		if(_win_ == rel->dsttype){
			arenawrite((void*)&rel->dstchip, (void*)&rel->srcchip, buf, len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int mahony_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mahony_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int mahony_create(struct element* ele, u8* url)
{
	say("@mahony_create\n");
	return 1;
}
