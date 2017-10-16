#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void main(int argc,char** argv);




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
