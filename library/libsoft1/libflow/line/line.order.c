#include "libsoft.h"
int reline_send(struct element* ele, u8* buf, int len)
{
	struct relation* rel;
	struct halfrel* cself;
	struct halfrel* cpeer;

	rel = ele->orel0;
	while(1){
		if(0 == rel)break;

		cself = (void*)&rel->dstchip;
		cpeer = (void*)&rel->srcchip;
		switch(rel->dsttype){
			case _art_:arterywrite(cself, cpeer, buf, len);break;
			case _act_: actorwrite(cself, cpeer, buf, len);break;
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}




int reline_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	say("@reline_read\n");
	return 0;
}
int reline_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	int j,k,cur;
	u8* tmp;
	struct element* ele;
	//say("@reline_write:%d\n", len);

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	//remain part
	cur = ele->len;
	tmp = ele->buf;
	if(cur){
		for(j=0;j<len;j++){
			tmp[cur] = buf[j];
			cur++;

			if('\n' == buf[j]){
				reline_send(ele, tmp, cur);
				cur = 0;

				j++;
				break;
			}
		}
		buf += j;
		len -= j;
	}
	if(len <= 0)return 0;

	//other part
	k = 0;
	for(j=0;j<len;j++){
		if('\n' == buf[j]){
			reline_send(ele, buf+k, j-k+1);
			k = j+1;
		}
	}
	if(k >= len)return 0;

	//
	for(j=0;j<len-k;j++)tmp[j] = buf[k+j];
	ele->len = j;
	return 0;
}
int reline_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reline_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int reline_create(struct element* ele, u8* url)
{
	say("@reline_create\n");
	ele->buf = memorycreate(0x1000);
	ele->len = 0;
	return 1;
}
