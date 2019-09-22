#include "libsoft.h"




int fftpcm_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int fftpcm_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct element* ele;
	say("@fftpcm_write\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

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




int fftpcm_create(struct element* ele, u8* url)
{
	say("@fftpcm_create\n");
	return 1;
}
