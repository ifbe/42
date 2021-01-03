#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void say(char* fmt,...);




void incomingfileowner(void* obj)
{
	say("incoming fileowner: %p\n", obj);
}




int readfile(void* obj, int fd, void* arg, int off, u8* buf, int len)
{
	return 0;
}
int writefile(void* obj, int fd, void* arg, int off, u8* buf, int len)
{
	return 0;
}
int stopfile()
{
	return 0;
}
int startfile(char* path)
{
	return 0;
}
int deletefile()
{
	return 0;
}
int createfile()
{
	return 0;
}




void initfilemgr()
{
	say("@initfilemgr\n");
}
void freefilemgr()
{
}