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
	struct element* ele;
	short* pcm;
	float* real;
	float* imag;
	say("@fftpcm_write\n");
	if(len < 2048)return 0;

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	pcm = buf;
	real = ele->buf;
	imag = ele->buf + 0x80000;
	for(j=0;j<1024;j++){
		real[j] = pcm[j] / 32768.0;
		imag[j] = 0.0;
	}
	fft(real, imag, 10);

	max = 0.0;
	for(j=1;j<512;j++){
		tmp = real[j]*real[j] + imag[j]*imag[j];
		if(max < tmp)max = tmp;
	}

	max = squareroot(max);
	max = max * 44100 / 1024;
	say("freq = %f\n", max);
	return 0;
}
int fftpcm_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int fftpcm_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int fftpcm_delete(struct element* ele)
{
	return 0;
}
int fftpcm_create(struct element* ele, u8* url)
{
	say("@fftpcm_create\n");
	ele->buf = memorycreate(0x100000, 0);
	return 1;
}
