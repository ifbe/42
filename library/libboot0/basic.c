#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void createcpu();
void deletecpu();
//
void creatememory();
void deletememory();
//
void createserial();
void deleteserial();




void basiccreate(void* type, u8* addr)
{
	int j;
	//createcpu();
	//creatememory();
	createserial();

	for(j=0;j<0x400000;j++)addr[j] = 0;
}
void basicdelete()
{
	deleteserial();
	//deletememory();
	//deletecpu();
}
