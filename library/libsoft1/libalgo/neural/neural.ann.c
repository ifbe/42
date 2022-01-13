#include "libsoft.h"
#define WEIGHT listptr.buf0
#define RESULT listptr.buf1




int ann_read(_obj* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, u8* image, int label)
{
	//say("@ann_read\n");

	int x,y;
	float* weight = art->WEIGHT;
	float* result = art->RESULT;

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
int ann_write(_obj* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, u8* image, int label)
{
	int x,y;
	float* weight;
	float* result;
	float delta[10];
	//say("@ann_read\n");

	weight = art->WEIGHT;
	result = art->RESULT;

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
int ann_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int ann_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@ann_attach\n");
	return 0;
}




int ann_search(_obj* ele)
{
	return 0;
}
int ann_modify(_obj* ele)
{
	return 0;
}
int ann_delete(_obj* ele)
{
	return 0;
}
int ann_create(_obj* ele, u8* url)
{
	int j;
	float* weight;
	float* result;
	say("@ann_create\n");

	weight = ele->WEIGHT = memorycreate(4*28*28*10, 0);
	for(j=0;j<28*28*10;j++)weight[j] = (getrandom()&0xfff)/1024.0;

	result = ele->RESULT = memorycreate(4*10, 0);
	return 1;
}
