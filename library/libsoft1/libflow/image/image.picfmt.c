#include "libsoft.h"
int copyfourcc(void*,void*);
int decstr2u32(void*,void*);
//
void bggr_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void bggr10_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void bgbgxgrgrx_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
//
void rggb_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void rggb10_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
//
void yuyv_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void uyvy_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
//
void yyyyuv_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
//
void yuvx_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
//
void yuyv_to_rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3);




struct perobj{
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




int picfmt_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int picfmt_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	struct perobj* per = (void*)art->priv_256b;
	//printmemory(buf, 16);
	//give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,len);

	if((_yuyv_ == per->srcfmt)&&(_yuvx_ == per->dstfmt)){
		yuyv_to_yuvx(buf, len, per->srcw, per->srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_uyvy_ == per->srcfmt)&&(_yuvx_ == per->dstfmt)){
		uyvy_to_yuvx(buf, len, per->srcw, per->srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_yyyyuv_ == per->srcfmt)&&(_yuvx_ == per->dstfmt)){
		yyyyuv_to_yuvx(buf, len, per->srcw, per->srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}

	if((_bggr_ == per->srcfmt)&&(_rgbx_ == per->dstfmt)){
		bggr_to_rgba(buf, len, per->srcw, per->srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_bg10_ == per->srcfmt)&&(_rgbx_ == per->dstfmt)){
		bggr10_to_rgba(buf, len, per->srcw, per->srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_test_ == per->srcfmt)&&(_rgbx_ == per->dstfmt)){
		bgbgxgrgrx_to_rgba(buf, len, per->srcw, per->srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}

	if((_rggb_ == per->srcfmt)&&(_rgbx_ == per->dstfmt)){
		rggb_to_rgba(buf, len, per->srcw, per->srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_rg10_ == per->srcfmt)&&(_rgbx_ == per->dstfmt)){
		rggb10_to_rgba(buf, len, per->srcw, per->srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}

	if((_yuvx_ == per->srcfmt)&&(_rgbx_ == per->dstfmt)){
		yuvx_to_rgba(buf, len, per->srcw, per->srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}

done:
/*	say("src(buf=%p,len=%x,w=%d,h=%d) -> dst(buf=%p,dstlen=%x,w=%d,h=%d)\n",
		buf, len, per->srcw, per->srch,
		per->dstbuf[0], per->dstlen, per->dstw, per->dsth);*/
	give_data_into_peer(art,_dst_, stack,sp, 0,0, per->dstbuf[0],per->dstlen);
	return 0;
}
int picfmt_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int picfmt_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int picfmt_delete(_obj* ele)
{
	return 0;
}
int picfmt_create(_obj* ele, u8* arg, int argc, char** argv)
{
	int j;
	say("@picfmt_create\n");

	struct perobj* per = (void*)ele->priv_256b;
	per->dstw = per->dsth = 0;

	for(j=0;j<argc;j++){
		say("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "srcfmt:", 7)){
			copyfourcc(&per->srcfmt, argv[j]+7);
			continue;
		}
		if(0 == ncmp(argv[j], "srcw:", 5)){
			decstr2u32(argv[j]+5, &per->srcw);
			continue;
		}
		if(0 == ncmp(argv[j], "srch:", 5)){
			decstr2u32(argv[j]+5, &per->srch);
			continue;
		}
		if(0 == ncmp(argv[j], "dstfmt:", 7)){
			copyfourcc(&per->dstfmt, argv[j]+7);
			continue;
		}
		if(0 == ncmp(argv[j], "dstw:", 5)){
			decstr2u32(argv[j]+5, &per->dstw);
			continue;
		}
		if(0 == ncmp(argv[j], "dsth:", 5)){
			decstr2u32(argv[j]+5, &per->dsth);
			continue;
		}
	}
	if(0 == per->dstw)per->dstw = per->srcw;
	if(0 == per->dsth)per->dsth = per->srch;

	per->dstlen = 4 * per->dstw * per->dsth;
	per->dstbuf[0] = memorycreate(per->dstlen, 4);

	say("src:fmt=%.4s,w=%d,h=%d\n", &per->srcfmt, per->srcw, per->srch);
	say("dst:fmt=%.4s,w=%d,h=%d\n", &per->dstfmt, per->dstw, per->dsth);
	say("buf:len=%x,ptr=%p\n", per->dstlen, per->dstbuf[0]);

	return 1;
}




int picfmt_free(void* obj)
{
	return 0;
}
void* picfmt_alloc()
{
	//return artery_alloc();
	return memorycreate(sizeof(struct item), sizeof(struct item));
}
