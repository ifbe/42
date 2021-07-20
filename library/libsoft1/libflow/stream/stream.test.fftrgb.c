#include "libsoft.h"




struct perobj{
	int* buf0;
	short* buf1;
	int len;
};




int fftrgb_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int fftrgb_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int x,y;
	int yy,uu,vv;
	int rr,gg,bb;
	u8* yuv;
	int* tmp;	//buf0
	short* pcm;	//buf3
	say("@fftrgb_write\n");

	struct perobj* perobj = (void*)art->priv_256b;

	//0. clean
	tmp = perobj->buf0;
	for(x=0;x<1024;x++)tmp[x] = 0;
//say("alive1\n");
	//1. yuv->rgb->tmp
	yuv = buf;
	for(y=0;y<480;y++){
		for(x=0;x<640;x++){
			yy = yuv[y*640*2 + x*2];
			uu = yuv[y*640*2 + (x&0xfffe)*2+1];
			vv = yuv[y*640*2 + (x&0xfffe)*2+3];

			rr = yy + (1.402 * (vv-128));
			gg = yy - (0.34413 * (uu-128)) - (0.71414 * (vv-128));
			bb = yy + (1.772 * (uu-128));

			if(rr <   0)rr = 0;
			if(rr > 255)rr = 255;
			if(gg <   0)gg = 0;
			if(gg > 255)gg = 255;
			if(bb <   0)bb = 0;
			if(bb > 255)bb = 255;
			tmp[(rr/26)*100 + (gg/26)*10 + bb/26] += 1;
		}
	}
//say("alive2\n");

	//2. tmp->pcm, send
	pcm = perobj->buf1 + perobj->len;
	perobj->len = (perobj->len + 1024*2) % 0x100000;
//say("alive3\n");
	for(x=0;x<1024;x++){
		pcm[x] = tmp[x]/10;
		//if(pcm[x] > 16)say("%d:%d\n", x, pcm[x]);
	}
//say("alive4\n");
	give_data_into_peer(art,_dst_, stack,sp, 0,0, pcm,1024*2);
	return 0;
}
int fftrgb_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int fftrgb_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int fftrgb_delete(_obj* ele)
{
	return 0;
}
int fftrgb_create(_obj* ele, u8* url)
{
	say("@fftrgb_create\n");

	struct perobj* perobj = (void*)ele->priv_256b;
	perobj->buf0 = memorycreate(0x100000, 0);
	perobj->buf1 = memorycreate(0x100000, 0);
	perobj->len = 0;
	return 1;
}
