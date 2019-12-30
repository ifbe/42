#include "libsoft.h"




int ann_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* image, int label)
{
	int x,y;
	struct artery* art;
	float* weight;
	float* result;
	//say("@ann_read\n");

	art = self->pchip;
	weight = art->buf0;
	result = art->buf1;

	for(y=0;y<10;y++){
		result[y] = 0;
		for(x=0;x<28*28;x++){
			result[y] += weight[28*28*y + x] * image[x];
		}
		result[y] /= 28*28*255;
		if(result[y] < 0.0)result[y] = 0.0;		//ReLU
	}

	return 0;
}
int ann_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* image, int label)
{
	int x,y;
	struct artery* art;
	float* weight;
	float* result;
	float delta[10];
	//say("@ann_read\n");

	art = self->pchip;
	weight = art->buf0;
	result = art->buf1;

	for(y=0;y<10;y++){
		result[y] = 0;
		for(x=0;x<28*28;x++){
			result[y] += weight[28*28*y + x] * image[x];
		}
		result[y] /= 28*28*255;
		if(result[y] < 0.0)result[y] = 0.0;		//ReLU
	}

	for(y=0;y<10;y++){
		if(y != label)delta[y] = - result[y];
		else delta[y] = 1.0 - result[y];
		//say("%d:%f\n",y,delta[y]);

		for(x=0;x<28*28;x++)weight[28*28*y + x] += delta[y]*image[x]/255.0/2.0;
	}
	return 0;
}
int ann_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int ann_start(struct halfrel* self, struct halfrel* peer)
{
	say("@ann_start\n");
	return 0;
}




int ann_search(struct artery* ele)
{
	return 0;
}
int ann_modify(struct artery* ele)
{
	return 0;
}
int ann_delete(struct artery* ele)
{
	return 0;
}
int ann_create(struct artery* ele, u8* url)
{
	int j;
	float* weight;
	float* result;
	say("@ann_create\n");

	weight = ele->buf0 = memorycreate(4*28*28*10, 0);
	for(j=0;j<28*28*10;j++)weight[j] = (getrandom()&0xfff)/1024.0;

	result = ele->buf1 = memorycreate(4*10, 0);
	return 1;
}
