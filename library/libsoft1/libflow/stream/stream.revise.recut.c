#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
int decstr2u64(u8* src, u64* dst);




struct perobj{
	u8* buf;
	u64 max;
	int enq;
	int deq;
};




int recut_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int recut_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	if(0 == art)return 0;

	int j,k;
	struct perobj* perobj = (void*)art->priv_256b;
	u8* cache = perobj->buf;
	int max = perobj->max;
	int deq = perobj->deq;
	int enq = perobj->enq;

	while(1){
		if(len <= 0)break;
		k = max-(enq%max);
		if(len < k){
			say("@recut_write?: %x+%x / %x\n", enq, len, max);
			for(j=0;j<len;j++)cache[enq+j] = buf[j];
			enq += len;
			break;
		}

		say("@recut_write!: %x+%x / %x\n", enq, k, max);
		for(j=0;j<k;j++)cache[enq+j] = buf[j];
		give_data_into_peer(art,_dst_, stack,sp, 0,0, cache+deq,max);

		buf += k;
		len -= k;

		deq += max;
		if(deq > max/2)deq = 0;
		enq = deq;
	}

	perobj->max = max;
	perobj->enq = enq;
	perobj->deq = deq;
	return 0;
}
int recut_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int recut_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int recut_create(_obj* ele, void* arg, int argc, u8** argv)
{
	say("@recut_create\n");

	struct perobj* perobj = (void*)ele->priv_256b;

	perobj->max = 640*480*2;
	if(arg)decstr2u64(arg, &perobj->max);

	perobj->buf = memoryalloc(perobj->max*2, 0);
	perobj->enq = 0;
	perobj->deq = 0;
	return 1;
}
