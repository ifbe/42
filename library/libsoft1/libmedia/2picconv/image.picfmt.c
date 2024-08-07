#include "libsoft.h"
int copyfourcc(void*,void*);
int copyeightcc(void*,void*);
int decstr2u32(void*,void*);
//
void bgr_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void rgb_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void bggr_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void bggr10_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
//
void bgbgxgrgrx_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth,
	float* ccm);
void bgbgxgrgrx_to_y4_u_v(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth,
	float* ccm);
//
void gbgbxrgrgx_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth,
	float* ccm);
void gbgbxrgrgx_to_y4_u_v(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth,
	float* ccm);
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
void y4_uv_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void y4_vu_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
//
void y4_u_v_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void y4_v_u_to_yuvx(
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
	struct kv88 kv[4];
	int zerocopy;
	int log;

	void* srcbuf[1];
	u64 srcfmt;
	u32 srclen;
	u32 srcw;
	u32 srch;

	void* dstbuf[1];
	u64 dstfmt;
	u32 dstlen;
	u32 dstw;
	u32 dsth;
};


void picfmt_copy_y(u8* dst, u8* src)
{
	logtoall("@picfmt_copy_y\n");
	int j;
	for(j=0;j<640*480;j++)dst[j] = src[j];
}
void picfmt_copy_u(u8* dst, u8* src)
{
	logtoall("@picfmt_copy_u\n");
	int j;
	dst = dst+640*480;
	for(j=0;j<640*480/4;j++)dst[j*2] = src[j*2];	//android u = u0 x u1 x u2 x u3 x ...
}
void picfmt_copy_v(u8* dst, u8* src)
{
	logtoall("@picfmt_copy_v\n");
	int j;
	dst = dst+640*480;
	for(j=0;j<640*480/4;j++)dst[j*2+1] = src[j*2];	//android v = v0 x v1 x v2 x v3 x ...
}
void picfmt_copy_uv(u8* dst, u8* src)
{
	logtoall("@picfmt_copy_uv\n");
	int j;
	for(j=0;j<640*480/2;j++)dst[j] = src[j];
}


int picfmt_take(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int cmd, void* buf, int len)
{
	return 0;
}
int picfmt_give(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int cmd, void* buf, int len)
{
	//logtoall("@picfmt_give: len=%d\n", len);
	struct perobj* per = (void*)art->priv_256b;

	//process metadata
	int ispart = 0;
	int srcw = per->srcw;
	int srch = per->srch;
	int srcstride = 0;
	u64 srcfmt = per->srcfmt;
	u64 ts = 0;
	float* m3 = 0;
	//logtoall("srcw=%d,srch=%d\n",srcw,srch);
	if(_kv88_ == cmd){
		int j;
		struct kv88* kv = (void*)arg;
		for(j=0;j<16;j++){
			//logtoall("key=%llx,val=%llx\n",kv[j].key, kv[j].val);
			if(kv[j].key <= 0x20)break;
			switch(kv[j].key){
			case '.':
				ispart = kv[j].val;
				break;
			case 'w':
				srcw = kv[j].val;
				break;
			case 'h':
				srch = kv[j].val;
				break;
			case _stride_:
				srcstride = kv[j].val;
				break;
			case 'f':
				srcfmt = kv[j].val;
				break;
			case 't':
				ts = kv[j].val;
				break;
			case hex16('m','3'):
				m3 = (float*)kv[j].val;
			}
		}
	}
	if((0 == srcw)|(0 == srch)){
		logtoall("wrong srcw=%d or srch=%d\n",srcw,srch);
		return 0;
	}

	//convert prepare
	if(ispart){
		if(0 == per->srcbuf[0]){
			per->srclen = srcw * srch * 3/2;
			per->srcbuf[0] = memoryalloc(per->srclen*2, 0);
		}
		if('y' == ispart){
			picfmt_copy_y(per->srcbuf[0], buf);
			return 0;
		}
		else if('u' == ispart){
			picfmt_copy_u(per->srcbuf[0], buf);
			return 0;
		}
		else if('v' == ispart){
			picfmt_copy_v(per->srcbuf[0], buf);
			buf = per->srcbuf[0];
			len = per->srclen;
			srcfmt = _y4_uv_;
		}
		else return 0;
	}

	//convert start
	if((0 == per->dstw)|(0 == per->dsth)){
		per->dstw = srcw;
		per->dsth = srch;
	}
	if(per->zerocopy){
		reading_data_from_peer(art, _dst_, 0, _buf_, &per->dstbuf, 1);
		if(per->log)logtoall("zerocopy:%p\n", per->dstbuf[0]);
	}
	if(0 == per->dstbuf[0]){
		per->dstlen = 4 * per->dstw * per->dsth;
		per->dstbuf[0] = memoryalloc(per->dstlen, 0);
	}

	if((_yuyv_ == srcfmt)&&(_yuvx_ == per->dstfmt)){
		if(srcstride)srcw = srcstride/2;
		yuyv_to_yuvx(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_uyvy_ == srcfmt)&&(_yuvx_ == per->dstfmt)){
		uyvy_to_yuvx(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_y4_uv_ == srcfmt)&&(_yuvx_ == per->dstfmt)){
		y4_uv_to_yuvx(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
/*	if((_y4_vu_ == srcfmt)&&(_yuvx_ == per->dstfmt)){
		y4_vu_to_yuvx(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}*/
	if((_y4_u_v_ == srcfmt)&&(_yuvx_ == per->dstfmt)){
		y4_u_v_to_yuvx(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
/*	if((_y4_v_u_ == srcfmt)&&(_yuvx_ == per->dstfmt)){
		y4_v_u_to_yuvx(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}*/

	if((_rgb_ == srcfmt)&&(_rgbx_ == per->dstfmt)){
		rgb_to_rgba(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_bgr_ == srcfmt)&&(_rgbx_ == per->dstfmt)){
		bgr_to_rgba(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_bggr_ == srcfmt)&&(_rgbx_ == per->dstfmt)){
		bggr_to_rgba(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_bg10_ == srcfmt)&&(_rgbx_ == per->dstfmt)){
		bggr10_to_rgba(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_pBAA_ == srcfmt)&&(_rgbx_ == per->dstfmt)){
		bgbgxgrgrx_to_rgba(
			buf, len, srcw, srch,
			per->dstbuf[0], per->dstlen, per->dstw, per->dsth,
			m3);
		goto done;
	}
	if((_pBAA_ == srcfmt)&&(_y4_u_v_ == per->dstfmt)){
		bgbgxgrgrx_to_y4_u_v(
			buf, len, srcw, srch,
			per->dstbuf[0], per->dstlen, per->dstw, per->dsth,
			m3);
		goto done;
	}
	if((_pGAA_ == srcfmt)&&(_rgbx_ == per->dstfmt)){
		gbgbxrgrgx_to_rgba(
			buf, len, srcw, srch,
			per->dstbuf[0], per->dstlen, per->dstw, per->dsth,
			m3);
		goto done;
	}
	if((_pGAA_ == srcfmt)&&(_y4_u_v_ == per->dstfmt)){
		gbgbxrgrgx_to_y4_u_v(
			buf, len, srcw, srch,
			per->dstbuf[0], per->dstlen, per->dstw, per->dsth,
			m3);
		goto done;
	}

	if((_rggb_ == srcfmt)&&(_rgbx_ == per->dstfmt)){
		rggb_to_rgba(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}
	if((_rg10_ == srcfmt)&&(_rgbx_ == per->dstfmt)){
		rggb10_to_rgba(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}

	if((_yuvx_ == srcfmt)&&(_rgbx_ == per->dstfmt)){
		yuvx_to_rgba(buf, len, srcw, srch,    per->dstbuf[0], per->dstlen, per->dstw, per->dsth);
		goto done;
	}

unknown:
	logtoall("picfmt_give unknown:srcfmt=%llx,dstfmt=%llx\n", srcfmt, per->dstfmt);
	printmemory(buf, 16);

done:
/*	logtoall("src(buf=%p,len=%x,w=%d,h=%d) -> dst(buf=%p,dstlen=%x,w=%d,h=%d)\n",
		buf, len, srcw, srch,
		per->dstbuf[0], per->dstlen, per->dstw, per->dsth);*/
	per->kv[0].key = 'w';per->kv[0].val = per->dstw;
	per->kv[1].key = 'h';per->kv[1].val = per->dsth;
	if(ts){per->kv[2].key = 't';per->kv[2].val = ts;}
	give_data_into_peer(art,_dst_, stack,sp, (p64)per->kv,_kv88_, per->dstbuf[0],per->dstlen);
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
	logtoall("@picfmt_create\n");

	struct perobj* per = (void*)ele->priv_256b;
	per->dstw = per->dsth = 0;

	for(j=0;j<argc;j++){
		logtoall("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "srcfmt:", 7)){
			copyeightcc(&per->srcfmt, argv[j]+7);
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
			copyeightcc(&per->dstfmt, argv[j]+7);
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
		if(0 == ncmp(argv[j], "zerocopy", 8)){
			per->zerocopy = 1;
			continue;
		}
		if(0 == ncmp(argv[j], "log:", 4)){
			per->log = 1;
		}
	}

	logtoall("src:fmt=%.4s,w=%d,h=%d\n", &per->srcfmt, per->srcw, per->srch);
	logtoall("dst:fmt=%.4s,w=%d,h=%d\n", &per->dstfmt, per->dstw, per->dsth);

	return 1;
}




int picfmt_free(void* obj)
{
	return 0;
}
void* picfmt_alloc()
{
	//return artery_alloc();
	return memoryalloc(sizeof(struct item), sizeof(struct item));
}
