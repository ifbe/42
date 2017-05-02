#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void main(int argc,char** argv);
//libsoft
void sleep_us(int);
u64 gettime();
//libhard
int snatch(void*);
int release(void*);
//libboot
void say(void*, ...);





//start must be at the first!!!!!!!!!!!!!!!!!
void start()
{
	main(0,0);
}




void death()
{
}
void* birth()
{
	//@16MB
	return (void*)0x1000000;
}
