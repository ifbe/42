#include "libsoft.h"
#define _s8_ hex16('s','8')
#define _s16_ hex32('s','1','6',0)
#define _s24_ hex32('s','2','4',0)
#define _s32_ hex32('s','3','2',0)
int copyfourcc(void*,void*);
int copyeightcc(void*,void*);
int decstr2u32(void*,void*);




struct perobj{
	struct kv88 kv[4];
	int zerocopy;
	int log;

	void* srcbuf[1];
	u32 srclen;
	u64 srcfmt;		//s32
	u32 srcendian;	//le
	u32 srcchan;	//1
	u32 srcfreq;	//48000

	void* dstbuf[1];
	u32 dstlen;
	u64 dstfmt;		//s16
	u32 dstendian;	//le
	u32 dstchan;	//1
	u32 dstfreq;	//44100
};


int pcmfmt_s32tos16(int* srcbuf, int srclen, short* dstbuf, int dstlen)
{
	int j,tmp;
	int count = srclen/4;
	for(j=0;j<count;j++){
		tmp = srcbuf[j] / 1024;
		if(tmp > 32767)tmp = 32767;
		if(tmp <-32767)tmp =-32767;
		dstbuf[j] = tmp;
	}
	return count*2;
}


int pcmfmt_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int cmd, void* buf, int len)
{
	return 0;
}
int pcmfmt_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int cmd, void* buf, int len)
{
	struct perobj* per = (void*)art->priv_256b;
	if(per->log)say("@pcmfmt_give:len=%x\n",len);

	int outlen;
	if( (_s32_ == per->srcfmt) && (_s16_ == per->dstfmt) ){
		if(0 == per->dstbuf[0]){
			per->dstlen = 0x100000;
			per->dstbuf[0] = memorycreate(per->dstlen, 4);
		}
		outlen = pcmfmt_s32tos16(buf,len, per->dstbuf[0], per->dstlen);
	}

	give_data_into_peer(art,_dst_, stack,sp, arg,cmd, per->dstbuf[0],outlen);
	return 0;
}
int pcmfmt_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int pcmfmt_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int pcmfmt_delete(_obj* ele)
{
	return 0;
}
int pcmfmt_create(_obj* ele, u8* arg, int argc, char** argv)
{
	say("@pcmfmt_create\n");

	struct perobj* per = (void*)ele->priv_256b;
	per->log = 0;

	per->dstfmt = _s16_;
	per->dstendian = 'l';
	per->dstchan = 1;
	per->dstfreq = 44100;

	int j;
	for(j=0;j<argc;j++){
		say("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "zerocopy", 8)){
			per->zerocopy = 1;
			continue;
		}
		else if(0 == ncmp(argv[j], "srcfmt:", 7)){
			copyeightcc(&per->srcfmt, argv[j]+7);
			continue;
		}
		else if(0 == ncmp(argv[j], "srcendian:", 10)){
			per->srcendian = argv[j][10];
			continue;
		}
		else if(0 == ncmp(argv[j], "srcchan:", 8)){
			decstr2u32(argv[j]+8, &per->srcchan);
			continue;
		}
		else if(0 == ncmp(argv[j], "srcfreq:", 8)){
			decstr2u32(argv[j]+8, &per->srcfreq);
			continue;
		}

		else if(0 == ncmp(argv[j], "dstfmt:", 7)){
			copyeightcc(&per->dstfmt, argv[j]+7);
			continue;
		}
		else if(0 == ncmp(argv[j], "dstendian:", 10)){
			per->dstendian = argv[j][10];
			continue;
		}
		else if(0 == ncmp(argv[j], "dstchan:", 8)){
			decstr2u32(argv[j]+8, &per->dstchan);
			continue;
		}
		else if(0 == ncmp(argv[j], "dstfreq:", 8)){
			decstr2u32(argv[j]+8, &per->dstfreq);
			continue;
		}
		else if(0 == ncmp(argv[j], "log:", 4)){
			per->log = 1;
			continue;
		}
	}

	per->srcbuf[0] = 0;
	per->srclen = 0;
	per->dstbuf[0] = 0;
	per->dstlen = 0;

	say("src:fmt=%.4s,endian=%c,chan=%d,freq=%d\n", &per->srcfmt, per->srcendian, per->srcchan, per->srcfreq);
	say("dst:fmt=%.4s,endian=%c,chan=%d,freq=%d\n", &per->dstfmt, per->dstendian, per->dstchan, per->dstfreq);

	return 1;
}




int pcmfmt_free(void* obj)
{
	return 0;
}
void* pcmfmt_alloc()
{
	//return artery_alloc();
	return memorycreate(sizeof(struct item), sizeof(struct item));
}
