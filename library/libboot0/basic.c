#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void initcpu();
void initmemory();
void initserial();




void basicinit(char* type,char* addr)
{
	//initcpu();
	//initmemory();
	initserial();
}
void basickill()
{
}




int bootevent(QWORD* first,QWORD* second)
{
	return 0;
}
