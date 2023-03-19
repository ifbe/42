#include "libsoft.h"
int openreadclose(void*, int, void*, int);
int openwriteclose(void*, int, void*, int);




struct privdata{
    float (*layer0_filter)[9];      //3x3, 10个
    float (*layer0_blas);           //10个
    float (*layer1_filter)[9];      //3x3, 10个
    float (*layer1_blas);           //10个
    //...
};
int cnn_forward()
{
	return 0;
}
int cnn_backward()
{
	return 0;
}




int cnn_read(_obj* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, u8* image, int label)
{
	//say("@cnn_read\n");
	cnn_forward();
	return 0;
}
int cnn_write(_obj* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, u8* image, int label)
{
	//say("@cnn_read\n");
	cnn_forward();
	cnn_backward();
	return 0;
}
int cnn_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int cnn_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@cnn_attach\n");
	return 0;
}




int cnn_search(_obj* ele)
{
	return 0;
}
int cnn_modify(_obj* ele)
{
	return 0;
}
int cnn_delete(_obj* ele)
{
	return 0;
}
int cnn_create(_obj* ele, u8* arg, int argc, u8** argv)
{
	return 1;
}
