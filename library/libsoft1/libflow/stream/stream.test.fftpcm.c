#include "libsoft.h"
double squareroot(double);
void fft(float* real, float* imag, int k);
void ifft(float* real, float* imag, int k);




struct perobj{
	void* buf0;
	void* buf1;
	int len;
};




int fftpcm_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int fftpcm_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int j;
	float maxidx,maxval;
	float magni,phase;
	short* pcm;
	float* real;
	float* imag;
	logtoall("@fftpcm_write\n");
	if(len < 2048)return 0;

	struct perobj* perobj = (void*)art->priv_256b;

	//1. pcm->complex
	pcm = buf;
	real = perobj->buf0;
	imag = perobj->buf0 + 0x80000;
	for(j=0;j<1024;j++){
		real[j] = pcm[j] / 32768.0;
		imag[j] = 0.0;
	}
	fft(real, imag, 10);

	//2.complex->pcm
	logtoall("len=%x\n", perobj->len);
	pcm = perobj->buf1 + perobj->len;
	perobj->len = (perobj->len+2048) % 0x100000;

	maxval = 0.0;
	for(j=0;j<1024;j++){
		magni = squareroot(real[j]*real[j] + imag[j]*imag[j])/1024;
		if(maxval < magni){
			maxidx = j;
			maxval = magni;
		}
		if(magni > 0.5){
			logtoall("%d:%f\n",j,magni);
			magni = 1.0;
		}

		//phase = arctanyx(imag[j], real[j]);

		pcm[j] = (short)(magni*32767.0);
		if(j < 1)continue;
	}

	maxidx = maxidx * 44100 / 1024;
	logtoall("freq = %f\n", maxidx);

	give_data_into_peer(art,_dst_, stack,sp, 0,0, pcm,1024*2);
/*
	pcm = buf;
	ifft(real, imag, 10);		//check if x==ifft(fft(x))
	for(j=0;j<16;j++){
		logtoall("%f, %f, %f\n", pcm[j]/32768.0, real[j], imag[j]);
	}
*/
	return 0;
}
int fftpcm_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int fftpcm_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int fftpcm_delete(_obj* ele)
{
	return 0;
}
int fftpcm_create(_obj* ele, u8* arg)
{
	logtoall("@fftpcm_create\n");

	struct perobj* perobj = (void*)ele->priv_256b;
	perobj->buf0 = memoryalloc(0x100000, 0);
	perobj->buf1 = memoryalloc(0x100000, 0);
	perobj->len = 0;
	return 1;
}
