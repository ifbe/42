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
//
void say(char* , ...);




void basiccreate(char* type,char* addr)
{
	//createcpu();
	//creatememory();
	createserial();
	//say("[0,4):createed basic\n");
}
void basicdelete()
{
	//say("[0,4):deleteing basic\n");
	deleteserial();
	//deletememory();
	//deletecpu();
}
