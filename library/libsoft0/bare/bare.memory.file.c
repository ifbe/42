#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void say(char* fmt,...);




//file名字，mem地址，文件内偏移，总字节数
int readfile(void* obj, int fd, void* arg, int off, u8* buf, int len)
{
}
int writefile(void* obj, int fd, void* arg, int off, u8* buf, int len)
{
}
void listfile(char* dest)
{
}
void choosefile()
{
}
int stopfile()
{
}
int startfile(char* path)
{
}
void deletefile()
{
}
void createfile()
{
}
