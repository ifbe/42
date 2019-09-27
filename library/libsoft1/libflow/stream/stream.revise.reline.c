#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)




int reline_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	float f[10];
	struct element* ele;
	say("@reline_read\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	relationread(ele, _src_, 0, 0, f, 10);
	return 0;
}
int reline_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int j,k,cur;
	u8* tmp;
	struct element* ele;
	say("@reline_write:%.4s\n", &self->flag);

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	switch(self->flag){
		case _dst_:{
			printmemory(buf, len);
			break;
		}
		case _src_:{
			//remain part
			cur = ele->len;
			tmp = ele->buf;
			if(cur){
				for(j=0;j<len;j++){
					tmp[cur] = buf[j];
					cur++;

					if('\n' == buf[j]){
						relationwrite(ele, _dst_, 0, 0, tmp, cur);
						cur = 0;

						j++;
						break;
					}
				}
				ele->len = cur;

				buf += j;
				len -= j;
			}
			if(len <= 0)return 0;

			//other part
			k = 0;
			for(j=0;j<len;j++){
				if('\n' == buf[j]){
					relationwrite(ele, _dst_, 0, 0, buf+k, j-k+1);
					k = j+1;
				}
			}
			if(k >= len)return 0;

			//
			for(j=0;j<len-k;j++)tmp[j] = buf[k+j];
			ele->len = j;

			break;
		}
	}

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
	ele->buf = memorycreate(0x1000, 0);
	ele->len = 0;
	return 1;
}
