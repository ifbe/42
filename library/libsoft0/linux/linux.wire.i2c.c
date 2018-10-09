#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>		//if you have problem
#include<linux/i2c-dev.h>
void printmemory(void*, int);
void say(void*, ...);




struct i2cjob{
	int fd;
	int dev;
	int reg;
	int len;
};
static struct i2cjob job[16];




int systemi2c_read(int fd, int addr, u8* buf, u8 len)
{
	struct i2c_msg messages[2];
	struct i2c_rdwr_ioctl_data packets;
	u8 dev = addr>>16;
	u8 reg = addr&0xffff;

	//out
	messages[0].addr  = dev;
	messages[0].flags = 0;
	messages[0].len   = 1;
	messages[0].buf   = &reg;

	//in
	messages[1].addr  = dev;
	messages[1].flags = I2C_M_RD;
	messages[1].len   = len;
	messages[1].buf   = buf;

	//send
	packets.msgs      = messages;
	packets.nmsgs     = 2; 
	if(ioctl(fd, I2C_RDWR, &packets) < 0)
	{
		//perror("Unable to send data");
		return -1;
	}

	return 1;
}
int systemi2c_write(int fd, int addr, u8* buf, u8 len)
{
	int j;
	u8 out[512];
	struct i2c_msg messages[1];
	struct i2c_rdwr_ioctl_data packets;
	u8 dev = addr>>16;
	u8 reg = addr&0xffff;

	//which,what0,what1,what2......
	out[0] = reg;
	for(j=0;j<len;j++)out[j+1] = buf[j];

	//message
	messages[0].addr  = dev;
	messages[0].flags = 0;
	messages[0].len   = len+1;
	messages[0].buf   = out;

	//transfer
	packets.msgs  = messages;
	packets.nmsgs = 1;
	j = ioctl(fd, I2C_RDWR, &packets);
	if(j < 0)
	{
		//perror("Unable to send data");
		return -1;
	}

	return 1;
}




static systemi2c_thread()
{
	int j,fd,addr;
	u8 buf[256];
	while(1)
	{
		fd = job[0].fd;
		addr = (job[0].dev<<16) | job[0].reg;
		systemi2c_read(fd, addr, buf, job[0].len);
printmemory(buf, job[0].len);
	}
}




int stopi2c(int fd, int dev, int reg, int len)
{
	return 0;
}
int starti2c(int fd, int dev, int reg, int len)
{
	job[0].fd = fd;
	job[0].dev = dev;
	job[0].reg = reg;
	job[0].len = len;
	threadcreate(systemi2c_thread, 0);
	return 0;
}
int deletei2c(int fd)
{
	return close(fd);
}
int createi2c(char* name, int flag)
{
	int j;
	u8 buf[256];

	for(j=0;j<128;j++)
	{
		if(name[j] <= 0x20)
		{
			buf[j] = 0;
			break;
		}
		else
		{
			buf[j] = name[j];
		}
	}

	say("i2c: %s\n", buf);
	return open(buf, O_RDWR);
}
/*
static int fp=-1;
static int where[4]={-1,-1,-1,-1};
static char that[16];
static unsigned char outbuf[16];








int systemi2c_list(char* towhere)
{
	int x;
	int y;
	int ret;
	unsigned char ch;

	//listbuses
	if(where[0]<0)
	{
		ret=system("ls /dev/i2c-*");
		return 0;
	}

	//listdevices
	if(where[1]<0)
	{
		for(y=0;y<8;y++)
		{
			for(x=0;x<16;x++)
			{
				ret=systemi2c_read((y*16)+x,0,&ch,1);

				if(ret>0)printf("%02x ",x+(y*16));
				else printf("-- ");
			}
			printf("\n");
		}
		return 1;
	}

	//listregisters
	if(where[2]<0)
	{
		for(y=0;y<16;y++)
		{
			for(x=0;x<16;x++)
			{
				ret=systemi2c_read(where[1],(y*16)+x,&ch,1);

				if(ret>0)printf("%02x ",ch);
				else printf("-- ");
			}
			printf("\n");
		}
		return 2;
	}

	//listbits
	if(where[3]<0)
	{
		return 3;
	}

	return -1;
}
int systemi2c_choose(int num,char* p)
{
	int ret;
//printf("num=%d,p=%s\n",num,p);

	//"cd"
	if( (num<0) && (p==0) )
	{
		say("@i2c");
		if(where[0]>=0)say("/0x%d",where[0]);
		if(where[1]>=0)say("/0x%x",where[1]);
		if(where[2]>=0)say("/0x%x",where[2]);
		say("\n");
		return 0;
	}

	//"cd .."
	if(num<0)
	{
		if(where[2]>=0)
		{
			where[2]=-1;
		}
		else if(where[1]>=0)
		{
			where[1]=-1;
		}
		else if(where[0]>=0)
		{
			if(fp > 0)
			{
				close(fp);
				where[0]=fp=-1;
			}
		}

		return 1;
	}

	//"cd 1" or "cd /dev/i2c-1"
	if(where[0]<0)
	{
		if(p!=0)fp = open(p,O_RDWR);
		else
		{
			snprintf(that,16,"/dev/i2c-%d",num);
			fp = open(that,O_RDWR);
		}

		if(fp <= 0)
		{
			printf("error open\n");
			return -1;
		}

		where[0]=num;
		where[1]=where[2]=where[3]=-1;
		return 0x11;
	}

	//select device
	if(where[1]<0)
	{
		ret=ioctl(fp,I2C_SLAVE,num);
		if(ret < 0)
		{
			printf("error ioctl\n");
			return -2;
		}

		where[1]=num;
		return 0x12;
	}

	return 0;
}




void systemi2c_start(char* p)
{
}
void systemi2c_stop()
{
}




void systemi2c_create()
{
	fp=-1;
	where[0]=where[1]=where[2]=where[3]=-1;
}
void systemi2c_delete()
{
	if(fp>0)
	{
		close(fp);

		fp=-1;
		where[0]=where[1]=where[2]=where[3]=-1;
	}
}
*/
