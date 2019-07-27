#include "libsoft.h"
int parsefv(float* fbuf, int flen, u8* sbuf, int slen);




int line2fv_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	say("@line2fv_read\n");
	return 0;
}
int line2fv_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	int cnt;
	float tmp[9];
	struct element* ele;
	struct relation* rel;
	struct halfrel* cself;
	struct halfrel* cpeer;
	//say("@line2fv_write:%d\n", len);
	//printmemory(buf, len);

	ele = (void*)(self->chip);
	if(0 == ele)return 0;


	cnt = parsefv(tmp, 9, buf, len);
	//say("fvcnt: %d\n", cnt);
	if((cnt != 3) && (cnt != 6) && (cnt != 9) ) {
		say("wrong fvcnt: %d\n", cnt);
		return 0;
	}


	rel = ele->orel0;
	while(1){
		if(0 == rel)break;

		cself = (void*)&rel->dstchip;
		cpeer = (void*)&rel->srcchip;
		switch(rel->dsttype){
			case _art_:arterywrite(cself, cpeer, tmp, cnt);break;
			case _act_: actorwrite(cself, cpeer, tmp, cnt);break;
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int line2fv_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int line2fv_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int line2fv_create(struct element* ele, u8* url)
{
	say("@line2fv_create\n");
	ele->buf = memorycreate(0x1000);
	ele->len = 0;
	return 1;
}
