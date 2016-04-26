#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<linux/i2c-dev.h>
#include<sys/ioctl.h>




//
static int fp=-1;
static int where[4];




int systemi2c_read(BYTE dev,BYTE reg,BYTE* buf,BYTE count)
{
	int ret;

	//select device
	ret=ioctl(fp,I2C_SLAVE,dev);
	if(ret < 0)
	{
		printf("error@systemi2c_read.ioctl\n");
		return;
	}

	//select register
	ret=write(fp,&reg,1);
	if(ret!=1)
	{
		printf("error@systemi2c_read.writereg:%x\n",ret);
		return -1;
	}

	//read data
	ret=read(fp,buf,count);
	if(ret<=0)
	{
		printf("error@systemi2c_read.readdata:%x\n",ret);
		return -2;
	}
	return 1;
}
int systemi2c_write(BYTE dev,BYTE reg,BYTE* buf,BYTE count)
{
	int ret;

	//select device
	ret=ioctl(fp,I2C_SLAVE,dev);
	if(ret < 0)
	{
		printf("error@systemi2c_write.ioctl\n");
		return;
	}

	//select register
	ret=write(fp,&reg,1);
	if(ret!=1)
	{
		printf("error@systemi2c_write.writereg:%x\n",ret);
		return -1;
	}
	//usleep(100);

	//write data
	ret=write(fp,buf,count);
	if(ret<=0)
	{
		printf("error@systemi2c_write.writedata:%x\n",ret);
		return -2;
	}
	return 1;
}




int systemi2c_list(char* towhere)
{
	int x;
	int y;
	int ret;
	char ch;

	//listbuses
	if(where[0]==0)
	{
		system("ls /dev/i2c-*");
		return 1;
	}

	//listdevices
	if(where[1]==0)
	{
		for(y=0;y<16;y++)
		{
		for(x=0;x<16;x++)
		{
			ret=systemi2c_read((y*16)+x,0,&ch,1);

			if(ret>0)printf("%.2x ",ch);
			else printf("-- ");
		}
		}
		printf("\n");
	}

	//listregisters
	if(where[2]==0)
	{
		for(y=0;y<16;y++)
		{
		for(x=0;x<16;x++)
		{
			ret=systemi2c_read(where[1],(y*16)+x,&ch,1);
			if(ret>0)printf("%.2x ",ch);
			else printf("-- ");
		}
		}
		printf("\n");
	}

	//listbits
	if(where[3]==0)
	{
	}
}
int systemi2c_choose(BYTE bus,BYTE dev,BYTE reg)
{
	int ret;

	//select bus

	//select device
	ret=ioctl(fp,I2C_SLAVE,dev);
	if(ret < 0)
	{
		printf("error ioctl\n");
		return;
	}

	//select register
}




void systemi2c_start(char* p)
{
	fp = open("/dev/i2c-1",O_RDWR);
	if(fp <= 0)
	{
		printf("error open\n");
		return;
	}

	where[0]=fp;
	where[1]=where[2]=where[3]=0;
}
void systemi2c_stop()
{
	if(fp > 0)
	{
		close(fp);
		where[0]=fp=0;
	}
}




void systemi2c_init()
{
}
void systemi2c_kill()
{
}
