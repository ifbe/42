#include "libuser.h"
void saveppmfromimg(char* name, int offs, u8* buf, int len, int width, int height);




int img2pbr_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int img2pbr_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
    return 0;
}
int img2pbr_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int img2pbr_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int img2pbr_create(_obj* ele, u8* arg)
{
	say("@img2pbr_create\n");
	//testing: merge albedo,normal,amocc into ppm
	int j;
	struct texture tex[3];
	u8* to = memoryalloc(2048*2048*4, 0);
	u8* tt = memoryalloc(2048*2048*4, 0);

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
	memoryfree(tt);
	memoryfree(to);
	return 1;
}
