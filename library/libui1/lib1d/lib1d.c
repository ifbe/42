#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void b8000_create(void*);
void cli_create(void*);
void tui_create(void*);




void lib1d_read()
{
}
void lib1d_write()
{
}
void lib1d_list()
{
}
void lib1d_choose()
{
}
void lib1d_stop()
{
}
void lib1d_start()
{
}
void lib1d_delete()
{
}
void lib1d_create(void* addr)
{
	//b8000_create(addr);
	cli_create(addr);
	//tui_create(addr);
}
