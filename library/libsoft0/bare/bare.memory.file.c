#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void say(char* fmt,...);




static void* filetable[16];
static int filecount = 0;
void filemanager_registersupplier(void* obj)
{
	say("@filemanager_registersupplier: [%d]=%p\n", filecount, obj);
	filetable[filecount] = obj;
	filecount += 1;
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




int filesearch(void* buf, int len)
{
	int j;
	say("@filesearch\n");
	for(j=0;j<filecount;j++){
		say("[%d]=%p\n", j, filetable[j]);
	}
	return 0;
}
int filemodify(void* buf, int len)
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