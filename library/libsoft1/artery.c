#define BYTE unsigned char
#define QWORD unsigned long long
#define DWORD unsigned int
//
void readfile(char* buf,QWORD sector,QWORD disk,DWORD count);
void readmemory(char* buf,QWORD sector,QWORD disk,DWORD count);
//
void bin_init(char*);
void fs_init(char*);
void pt_init(char*);




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void rawlist()
{
}
void rawswitch()
{
}
void rawread(char* buf,QWORD sector,QWORD disk,DWORD count)
{
	readfile(buf,sector,disk,count);
}
void rawwrite(char* buf,QWORD sector,QWORD disk,DWORD count)
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void openartery(char* what)
{
	//选一个操作者
}
void closeartery()
{
}
void initartery(char* world)
{
	bin_init(world);	//1
        fs_init(world);		//2
}
void killartery()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
