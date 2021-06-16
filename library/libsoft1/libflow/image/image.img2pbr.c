#include "libuser.h"
void saveppmfromimg(char* name, int offs, u8* buf, int len, int width, int height);




int img2pbr_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int img2pbr_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
    return 0;
}
int img2pbr_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int img2pbr_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int img2pbr_create(struct artery* ele, u8* url)
{
	say("@img2pbr_create\n");
	//testing: merge albedo,normal,amocc into ppm
	int j;
	struct texture tex[3];
	u8* to = memorycreate(2048*2048*4, 0);
	u8* tt = memorycreate(2048*2048*4, 0);

	tex[0].data = tt;
	loadtexfromfile(&tex[0], "/Users/ifbe/Downloads/rustiron/_metal.png");
	for(j=0;j<2048*2048*4;j+=4){
		to[j+0] = tt[j+0];
	}

	tex[1].data = tt;
	loadtexfromfile(&tex[1], "/Users/ifbe/Downloads/rustiron/_rough.png");
	for(j=0;j<2048*2048*4;j+=4){
		to[j+1] = tt[j+1];
	}

	tex[2].data = tt;
	loadtexfromfile(&tex[2], "/Users/ifbe/Downloads/rustiron/_ao.png");
	for(j=0;j<2048*2048*4;j+=4){
		to[j+2] = tt[j+2];
	}

    saveppmfromimg("/Users/ifbe/Downloads/rustiron/matter.ppm", 0, to, 4, 2048, 2048);
	memorydelete(tt);
	memorydelete(to);
	return 1;
}
