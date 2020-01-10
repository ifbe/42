#include "libsoft.h"
double squareroot(double);
void fft(float* real, float* imag, int k);
void ifft(float* real, float* imag, int k);




int fftpcm_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int fftpcm_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int j;
	float tmp,max;
	struct artery* ele;
	short* pcm;
	float* real;
	float* imag;
	say("@fftpcm_write\n");
	if(len < 2048)return 0;

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	//1. pcm->complex
	pcm = buf;
	real = ele->buf0;
	imag = ele->buf0 + 0x80000;
	for(j=0;j<1024;j++){
		real[j] = pcm[j] / 32768.0;
		imag[j] = 0.0;
	}
	fft(real, imag, 10);

	//2.complex->pcm
	pcm = ele->buf1 + ele->len;
	ele->len = (ele->len+2048) % 0x100000;

	max = 0.0;
	for(j=0;j<1024;j++){
		tmp = real[j]*real[j] + imag[j]*imag[j];
		pcm[j] = (short)(squareroot(tmp)*100.0);
		if(j < 1)continue;
		if(max < tmp)max = tmp;
	}

	max = squareroot(max);
	max = max * 44100 / 1024;
	say("freq = %f\n", max);

	relationwrite(ele, _dst_, 0, 0, pcm, 1024*2);
	return 0;
}
int fftpcm_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int fftpcm_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int fftpcm_delete(struct artery* ele)
{
	return 0;
}
int fftpcm_create(struct artery* ele, u8* url)
{
	say("@fftpcm_create\n");
	ele->buf0 = memorycreate(0x100000, 0);

	ele->buf1 = memorycreate(0x100000, 0);
	ele->len = 0;
	return 1;
}
