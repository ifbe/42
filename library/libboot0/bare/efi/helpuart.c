#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int bootservice_input(void* buf);
int bootservice_output(char* buf, int len);




static int enable = 0;




int read8250_one(u8* buf)
{
	if(0 == enable)return -1;

	return bootservice_input(buf);
}
int lowlevel_input(u8* buf, int len)
{
	if(0 == enable)return -1;

	int j,ret;
	for(j=0;j<len;j++){
		ret = read8250_one(buf+j);
		if(ret == 0)break;
	}

	return j;
}




int write8250_one(u8 data)
{
	if(0 == enable)return -1;
	return 0;
}
int lowlevel_output(char* buf, int len)
{
	if(0 == enable)return -1;

	bootservice_output(buf, len);

	return 0;
}




void lowlevelserial_disable(){
	enable = 0;
}




void freeserial()
{
	enable = 0;
}
void initserial()
{
	enable = 1;
	lowlevel_output("42!\n", 4);
}
