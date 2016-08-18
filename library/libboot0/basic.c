#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
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




int bootevent(QWORD* first,QWORD* second)
{
	return 0;
}




void basiccreate(char* type,char* addr)
{
	//createcpu();
	//creatememory();
	createserial();
	say("[0,4):createed basic\n");
}
void basicdelete()
{
	say("[0,4):deleteing basic\n");
	deleteserial();
	//deletememory();
	//deletecpu();
}
