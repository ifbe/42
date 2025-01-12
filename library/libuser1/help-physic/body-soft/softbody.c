#include "libsoft.h"




int softsimu_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("@softsimu_read:%.4s\n",&foot);
	return 0;
}
int softsimu_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	return 0;
}
int softsimu_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int softsimu_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int softsimu_reading(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int softsimu_writing(_obj* ent,void* foot, p64 arg,int key, u8* buf,int len)
{
	return 0;
}
int softsimu_delete(_obj* ent)
{
	return 0;
}
int softsimu_create(_obj* ent, void* str, int argc, u8** argv)
{
	return 0;
}
