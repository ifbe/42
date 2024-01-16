#include "libuser.h"
void hexedit_init(void*);
void overview_init(void*);
void drawvt100_create(void*);
//
void initutf8(void*);
int openreadclose(void* name, int off, void* buf, int len);
int openwriteclose(void* name, int off, void* buf, int len);




static u8* utf8data = 0;
static u16* bgmdata = 0;
//static struct fstyle def2d;
static struct fstyle def3d;




void asset_create()
{
	int j,k;
	if(0 == utf8data)
	{
		//malloc
		utf8data = memoryalloc(0x200000, 0);
		if(0 == utf8data)return;

		//read
		j = openreadclose("datafile/unicode.raw", 0, utf8data, 0x200000);
		if(j < 0x200000){
			logtoall("error@unicode\n");
			memoryfree(utf8data);
			return;
		}

		//config
		for(j=0;j<0x200000;j++)
		{
			k = utf8data[j];
			k = ((k&0x0f)<<4) | ((k&0xf0)>>4);
			k = ((k&0x33)<<2) | ((k&0xcc)>>2);
			k = ((k&0x55)<<1) | ((k&0xaa)>>1);
			utf8data[j] = k;
		}
		initutf8(utf8data);
	}
}
void asset_delete()
{
}




void formem_exit()
{
}
void formem_init(void* addr)
{
	//asset_create();

	hexedit_init(addr);

	overview_init(addr);

	drawvt100_create(addr);
}
