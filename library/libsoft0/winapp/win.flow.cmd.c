#include "libsoft.h"




static _obj* g_obj = 0;
int inituart(void* addr)
{
	g_obj = addr;
}
void freeshell()
{
}




int shell_insert()
{
	return 0;
}
int shell_remove()
{
	return 0;
}
int shell_search(char* p)
{
	return 0;
}
int shell_modify(char* p)
{
	return 0;
}




_obj* shell_create(char* p)
{
	return 0;
}
int shell_delete(_obj* fd)
{
	say("!!!this func is wrong!!!\n");
	return 0;
}
int shell_reader(_obj* oo,int xx, void* arg,int off, void* buf,int len)
{
	return 0;
}
int shell_writer(_obj* oo,int xx, void* arg,int off, void* buf,int len)
{
	return 0;
}




int shell_attach()
{
	return 0;
}
int shell_detach()
{
	return 0;
}
int shell_takeby(_obj* oo,int xx, void* arg,int off, void* buf,int len)
{
	return 0;
}
int shell_giveby(_obj* oo,int xx, void* arg,int off, void* buf,int len)
{
	return 0;
}