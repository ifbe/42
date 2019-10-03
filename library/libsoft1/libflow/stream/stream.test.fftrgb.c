#include "libsoft.h"




int fftrgb_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int fftrgb_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int x,y;
	int yy,uu,vv;
	int rr,gg,bb;
	u8* yuv;
	struct element* ele;

	int* tmp;	//buf0
	short* pcm;	//buf3
	say("@fftrgb_write\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	//0. clean
	tmp = ele->buf0;
	for(x=0;x<1024;x++)tmp[x] = 0;

	//1. yuv->rgb->tmp
	yuv = buf;
	for(y=0;y<480;y++){
		for(x=0;x<640;x++){
			yy = yuv[y*640 + x];
			uu = yuv[y*640 + (x&0xfffe)+1];
			vv = yuv[y*640 + (x&0xfffe)+3];
			rr = yy + (1.370705 * (vv-128));
			gg = yy - (0.698001 * (vv-128)) - (0.337633 * (uu-128));
			bb = yy + (1.732446 * (uu-128));
			tmp[(rr/26)*100 + (gg/26)*10 + bb/26] += 1;
		}
	}

	//2. tmp->pcm, send
	pcm = ele->buf + ele->len;
	ele->len = (ele->len + 1024*2) % 0x100000;

	for(x=0;x<1024;x++){
		pcm[x] = tmp[x]/10;
		if(pcm[x] > 16)say("%d:%d\n", x, pcm[x]);
	}
	relationwrite(ele, _dst_, 0, 0, pcm, 1024*2);
	return 0;
}
int fftrgb_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int fftrgb_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int fftrgb_delete(struct element* ele)
{
	return 0;
}
int fftrgb_create(struct element* ele, u8* url)
{
	say("@fftrgb_create\n");
	ele->buf0 = memorycreate(0x100000, 0);

	ele->buf = memorycreate(0x100000, 0);
	ele->len = 0;
	return 1;
}
