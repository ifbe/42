#include "libuser.h"
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
void lib4d_create(void*, void*);
void lib4d_delete();




void mine_free()
{
	lib4d_delete();
	lib3d_delete();
	lib2d_delete();
	lib1d_delete();
}
void mine_init(void* addr)
{
	lib1d_create(addr, 0);
	lib2d_create(addr, 0);
	lib3d_create(addr, 0);
	lib4d_create(addr, 0);
}