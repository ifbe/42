#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void menu_create(u8*,u8*);
void menu_delete();
void virtkbd_create(u8*,u8*);
void virtkbd_delete();




void special_create(void* addr, void* temp)
{
	//menu_create();
	//virtkbd_create();
}
void special_delete()
{
	menu_delete();
	virtkbd_delete();
}