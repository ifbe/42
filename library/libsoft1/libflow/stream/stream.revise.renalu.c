#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
int decstr2u64(u8* src, u64* dst);




struct perobj{
	u8* buf;
	int enq;
	int deq;
};




int renalu_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int renalu_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	say("@renalu_write\n");
	printmemory(buf, 16);
	if(0 == art)return 0;

	struct perobj* per = (void*)art->priv_256b;
	u8* bak = per->buf;		//use [0x10000,0xeffff]
	int enq = per->enq;
	int deq = per->deq;
	int j,restart=1;
	while(restart){
say("000000\n");
		restart = 0;
		for(j=0;j<len;j++){
			bak[enq+j] = buf[j];

			//find 0001
			if(1 != buf[j])continue;
			if(0 != bak[enq+j-1])continue;
			if(0 != bak[enq+j-2])continue;
			if(0 != bak[enq+j-3])continue;
say("11111\n");
			//send prev pack
			if(0 == deq){
				deq = enq+j-3;
				continue;
			}
			give_data_into_peer(art,_dst_, stack,sp, 0,0, bak+deq,enq+j-3-deq);
say("222222\n");
			if(enq+j >= 0xf0000){
				bak[0x10000+0] = bak[0x10000+1] = bak[0x10000+2] = 0;
				bak[0x10000+3] = 1;
				deq = 0x10000;
				enq = 0x10004;
				buf += j+1;
				len -= j+1;
				restart = 1;
				break;
			}
			else{
				deq = enq+j-3;
			}
		}
	}
	per->enq = enq+len;
	per->deq = deq;
	return 0;
}
int renalu_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int renalu_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int renalu_create(_obj* ele, void* arg, int argc, u8** argv)
{
	say("@renalu_create\n");

	struct perobj* perobj = (void*)ele->priv_256b;

	perobj->buf = memoryalloc(0x100000, 0);
	perobj->enq = 0x10000;
	perobj->deq = 0;
	return 1;
}
int renalu_delete(_obj* obj)
{
	return 0;
}