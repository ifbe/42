#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void say(char* , ...);
void initcpu();
void initmemory();
void initserial();




int bootevent(QWORD* first,QWORD* second)
{
	return 0;
}




void basicinit(char* type,char* addr)
{
	//initcpu();
	//initmemory();
	initserial();
	say("[0,4):inited basic\n");
}
void basickill()
{
	say("[0,4):killing basic\n");
	killserial();
	//killmemory();
	//killcpu();
}
