#include "libsoft.h"




int madgwick_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct element* ele;
	struct relation* rel;
	say("@madgwick_read\n");

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
int madgwick_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct element* ele;
	struct relation* rel;
	say("@madgwick_write\n");

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
int madgwick_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int madgwick_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int madgwick_create(struct element* ele, u8* url)
{
	say("@madgwick_create\n");
	return 1;
}
