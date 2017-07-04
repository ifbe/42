#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void navi_create(void*);
void navi_delete();
void vkbd_create(void*);
void vkbd_delete();
void vt100_create(void*);
void vt100_delete();




void lib2d_read()
{
}
void lib2d_write()
{
}
void lib2d_list()
{
}
void lib2d_choose()
{
}
void lib2d_stop()
{
}
void lib2d_start()
{
}
void lib2d_delete()
{
	vt100_delete();
	vkbd_delete();
	navi_delete();
}
void lib2d_create(void* base)
{
	navi_create(base);
	vkbd_create(base);
	vt100_create(base);
}
