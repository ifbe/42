#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void say(char* fmt,...);




//file名字，mem地址，文件内偏移，总字节数
int writefile(u64 fd, u64 off, u8* buf, u64 len)
{
}
int readfile(u64 fd, u64 off, u8* buf, u64 len)
{
}
void listfile(char* dest)
{
}
void choosefile()
{
}
int startfile(char* path)
{
}
void stopfile()
{
}
void createfile()
{
}
void deletefile()
{
}
