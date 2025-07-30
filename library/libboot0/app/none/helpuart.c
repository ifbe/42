#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<termios.h>
#include<sys/ioctl.h>
#include<sys/select.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




int lowlevel_input()
{
	u8 buf[8];
	while(1)
	{
		buf[0] = getchar();
		if( (buf[0] == 0) | (buf[0]==0xff) )
		{
			usleep(10000);
			continue;
		}
		if(buf[0] != 0x1b)return buf[0];

		buf[1] = getchar();
		if(buf[1] == 0xff)return 0x1b;

		if(buf[1] == 0x5b)
		{
			buf[2] = getchar();

			if( (buf[2]>=0x41) && (buf[2]<=0x44) )
			{
				buf[3] = 0;
				return *(int*)buf;
			}
		}//5b
		else continue;
	}
}
void lowlevel_output(char* buf, int len)
{
	//printf("%.*s", len, buf);
	int ret,retry=0;
	while(1){
		ret = write(1, buf, len);
		if(ret == len)return;
		if(ret > 0){
			buf += ret;
			len -= ret;
			continue;
		}

		usleep(1);
		retry++;
		if(retry > 999)break;
	}
	fprintf(stderr, "errno=%d,ret=%d,len=%d@stdout\n",errno,ret,len);
	fprintf(stderr, "{[(%.*s)]}\n", len, buf);
}




void freeserial()
{
}
void initserial()
{
}
