#include "libsoft.h"
#define WEIGHT listptr.buf0
#define RESULT listptr.buf1
int openreadclose(void*, int, void*, int);
int openwriteclose(void*, int, void*, int);




int ann_forward(u8* image, int label, float* weight, float* result)
{
	int x,y;
	float* bias = &weight[28*28*10];
	for(y=0;y<10;y++){
		result[y] = 0;
		for(x=0;x<28*28;x++){
			result[y] += weight[28*28*y + x] * image[x] + bias[28*28*y + x];
		}
		result[y] /= 28*28*255;
		if(result[y] < 0.0)result[y] = 0.0;		//ReLU
	}

	return 0;
}



/*
L = (result-real)*(result-real) = result*result - 2*result*real + real*real
result = w * x + b
Ltoresult = 2*result-2*real
resulttow = x
resulttob = 1
Ltow = Ltoresult * resulttow
Ltob = Ltoresult
*/
int ann_backward(u8* image, int label, float* weight, float* result)
{
	int x,y;
	float real, L_to_R, R_to_W, R_to_B, L_to_W, L_to_B;
	float* bias = &weight[28*28*10];
	for(y=0;y<10;y++){
		real = (y==label) ? 1.0 : 0.0;
		L_to_R = result[y] - real;
		for(x=0;x<28*28;x++){
			L_to_W = L_to_R * (image[x]/255.0);
			L_to_B = L_to_R;
			weight[28*28*y + x] += -0.1 * L_to_W;
			bias[28*28*y + x] += -0.1 * L_to_B;
		}
	}
	return 0;
}




int ann_read(_obj* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, u8* image, int label)
{
	//say("@ann_read\n");
	float* weight = art->WEIGHT;
	float* result = art->RESULT;
	ann_forward(image, label, weight, result);
	return 0;
}
int ann_write(_obj* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, u8* image, int label)
{
	//say("@ann_read\n");
	float* weight = art->WEIGHT;
	float* result = art->RESULT;
	if(_ev_ == key){
		say("onevent:%x\n",image[0]);
		switch(image[0]){
		case 'r':
			openreadclose("myvar.bin", 0, weight, 4*28*28*10*2);
			break;
		case 'w':
			openwriteclose("myvar.bin", 0, weight, 4*28*28*10*2);
			break;
		}
	}
	else{
		ann_forward(image, label, weight, result);
		ann_backward(image, label, weight, result);
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

	weight = ele->WEIGHT = memorycreate(sizeof(float)*28*28*10 * 2, 0);
	for(j=0;j<28*28*10*2;j++)weight[j] = (random_read()&0xfff)/1024.0;

	result = ele->RESULT = memorycreate(4*10, 0);
	return 1;
}
