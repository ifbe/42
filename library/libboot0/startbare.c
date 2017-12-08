#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define PORT 0x3f8
void main(int argc,char** argv);
void out8(u32 port, u8 data);




//start must be at the first!!!!!!!!!!!!!!!!!
void start()
{
	out8(PORT, 'a');
	out8(PORT, 'b');
	out8(PORT, 'c');
	out8(PORT, 'd');
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
