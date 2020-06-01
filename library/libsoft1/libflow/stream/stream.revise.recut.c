#include "libsoft.h"
#define MAX data1
#define ENQ data2
#define DEQ data3
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
int decstr2u64(u8* src, u64* dst);




int recut_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int recut_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	if(0 == art)return 0;

	int j,k;
	int deq = art->DEQ;
	int enq = art->ENQ;
	int max = art->MAX;
	u8* cache = art->buf0;

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
		if(deq > 0x100000/2)deq = 0;
		enq = deq;
	}

	art->MAX = max;
	art->ENQ = enq;
	art->DEQ = deq;
	return 0;
}
int recut_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int recut_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int recut_create(struct artery* ele, void* arg, int argc, u8** argv)
{
	say("@recut_create\n");
	ele->buf0 = memorycreate(0x100000, 0);

	ele->MAX = 640*480*2;
	if(arg)decstr2u64(arg, &ele->MAX);

	ele->ENQ = 0;
	ele->DEQ = 0;
	return 1;
}
