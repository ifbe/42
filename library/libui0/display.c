#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void windowcreate();
void windowdelete();
void say(char*,...);




void displaycreate(char* type,char* addr)
{
	if(type!=0)return;

	//
	windowcreate();

	//
	say("[c,f):createed display\n");
}
void displaydelete()
{
	//
	say("[c,f):deleteing display\n");

	//1024*1024*4
	windowdelete();
}
