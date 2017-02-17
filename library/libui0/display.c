#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void windowcreate(void* list);
void windowdelete();
void say(void*,...);




//
static u8* mega0;
static u8* mega1;
static u8* mega2;
static u8* mega3;




void displaycreate(u8* type, u8* addr)
{
	int j;
	if(type!=0)return;

	//where
	mega0=addr;
	mega1=addr+0x100000;
	mega2=addr+0x200000;
	mega3=addr+0x300000;

	//clean [0x80000,0xfffff]
	for(j=0;j<0x100000;j++)mega0[j]=0;

	//
	windowcreate(mega0);

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
