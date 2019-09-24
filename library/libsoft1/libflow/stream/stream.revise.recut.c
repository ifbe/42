#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)




int recut_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int recut_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int j,ret;
	u8* mem;
	struct element* ele;

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	mem = ele->buf;
	ret = ele->len;
	say("@recut_write: %x + %x = %x\n", ret, len, ret+len);

	j = 0;
	while(1){
		if(ret == 640*480*2){
			say("sending\n");
			relationwrite(ele, _dst_, 0, 0, mem, 640*480*2);
			ret = 0;
			continue;
		}
		if(j >= len)break;

		mem[ret] = buf[j];
		ret++;
		j++;
	}
	ele->len = ret;
	return 0;
}
int recut_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int recut_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int recut_create(struct element* ele, u8* url)
{
	say("@recut_create\n");
	ele->buf = memorycreate(0x100000, 0);
	ele->len = 0;
	return 1;
}
