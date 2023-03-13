#include "libsoft.h"
int copyfourcc(void*,void*);
int decstr2u32(void*,void*);




struct perobj{
	struct kv88 kv[4];

	void* srcbuf[1];
	u32 srclen;
	u32 srcfmt;
	u32 srcw;
	u32 srch;

	void* dstbuf[1];
	u32 dstlen;
	u32 dstfmt;
	u32 dstw;
	u32 dsth;
};


int sobel_convert(u8* src, u8* dst)
{
	int x,y;
	int da,db;
	u8* sa;
	u8* sb;
	u8* sc;
	u8* kdst;
	for(y=1;y<480-1;y++){
		sa = &src[(y-1)*640*4];
		sb = &src[(y+0)*640*4];
		sc = &src[(y+1)*640*4];
		kdst = &dst[y*640*4];
		for(x=1;x<640-1;x++){
			da = (
				- sa[(x-1)*4] + sa[(x+1)*4]
				- 2*sb[(x-1)*4] + 2*sb[(x+1)*4]
				- sc[(x-1)*4] + sc[(x+1)*4]
			);
			db = (
				- sa[(x-1)*4] - 2*sa[x*4] - sa[(x+1)*4]
				+ sc[(x-1)*4] + 2*sc[x*4] + sc[(x+1)*4]
			);
			if(da<0)da = -da;
			if(db<0)db = -db;
			kdst[x*4+0] = (da+db) / 2;
			kdst[x*4+1] = 0x80;
			kdst[x*4+2] = 0x80;
		}
	}
	return 0;
}


int sobel_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int cmd, void* buf, int len)
{
	return 0;
}
int sobel_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int cmd, void* buf, int len)
{
	struct perobj* per = (void*)art->priv_256b;

	//process metadata
	int srcfmt = per->srcfmt;
	int srcw = per->srcw;
	int srch = per->srch;
	if(_kv88_ == cmd){
		int j;
		struct kv88* kv = arg;
		for(j=0;j<16;j++){
			if(kv[j].key <= 0x20)break;
			switch(kv[j].key){
			case 'w':
				srcw = kv[j].val;
				break;
			case 'h':
				srch = kv[j].val;
				break;
			case 'f':
				srcfmt = kv[j].val;
				break;
			}
		}
	}
	if((0 == srcw)|(0 == srch)){
		say("wrong srcw or srch\n");
		return 0;
	}

	//convert prepare
	if((0 == per->dstw)|(0 == per->dsth)){
		per->dstw = srcw;
		per->dsth = srch;
	}
	if(0 == per->dstbuf[0]){
		per->dstlen = 4 * per->dstw * per->dsth;
		per->dstbuf[0] = memorycreate(per->dstlen, 4);
	}

	sobel_convert(buf, per->dstbuf[0]);

done:
	per->kv[0].key = 'w';per->kv[0].val = per->dstw;
	per->kv[1].key = 'h';per->kv[1].val = per->dsth;
	give_data_into_peer(art,_dst_, stack,sp, per->kv,_kv88_, per->dstbuf[0],per->dstlen);
	return 0;
}
int sobel_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int sobel_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int sobel_delete(_obj* ele)
{
	return 0;
}
int sobel_create(_obj* ele, u8* arg, int argc, char** argv)
{
	int j;
	say("@sobel_create\n");

	struct perobj* per = (void*)ele->priv_256b;
	per->dstw = per->dsth = 0;

	for(j=0;j<argc;j++){
		say("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "srcfmt:", 7)){
			copyfourcc(&per->srcfmt, argv[j]+7);
			continue;
		}
	}

	return 1;
}




int sobel_free(void* obj)
{
	return 0;
}
void* sobel_alloc()
{
	//return artery_alloc();
	return memorycreate(sizeof(struct item), sizeof(struct item));
}
