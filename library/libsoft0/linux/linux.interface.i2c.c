#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
//#include<linux/i2c.h>		//if you have problem
#include<linux/i2c-dev.h>
void say(char*,...);




//
static int fp=-1;
static int where[4]={-1,-1,-1,-1};
static unsigned char that[16];
static unsigned char outbuf[16];




int systemi2c_read(BYTE dev,BYTE reg,BYTE* buf,BYTE count)
{
	struct i2c_msg messages[2];
	struct i2c_rdwr_ioctl_data packets;

	//out
	outbuf[0] = reg;  
	messages[0].addr  = dev;
	messages[0].flags = 0;
	messages[0].len   = 1;
	messages[0].buf   = outbuf;

	//in
	messages[1].addr  = dev;
	messages[1].flags = I2C_M_RD;
	messages[1].len   = count;
	messages[1].buf   = buf;

	//send
	packets.msgs      = messages;
	packets.nmsgs     = 2; 
	if(ioctl(fp, I2C_RDWR, &packets) < 0)
	{
		//perror("Unable to send data");
		return -1;
	}

	return 1;
}
int systemi2c_write(BYTE dev,BYTE reg,BYTE* buf,BYTE count)
{
	int ret;
	struct i2c_msg messages[1];
	struct i2c_rdwr_ioctl_data packets;

	//which,what0,what1,what2......
	outbuf[0] = reg;
	for(ret=0;ret<count;ret++)
	{
		outbuf[ret+1] = buf[ret];
	}

	//message
	messages[0].addr  = dev;
	messages[0].flags = 0;
	messages[0].len   = count+1;
	messages[0].buf   = outbuf;

	//transfer
	packets.msgs  = messages;
	packets.nmsgs = 1;
	ret=ioctl(fp, I2C_RDWR, &packets);
	if(ret<0)
	{
		//perror("Unable to send data");
		return -1;
	}

	return 1;
}




int systemi2c_list(char* towhere)
{
	char ch;
	int x;
	int y;
	int ret;

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

				if(ret>0)printf("%.2x ",x+(y*16));
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

				if(ret>0)printf("%.2x ",ch);
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
}




void systemi2c_start(char* p)
{
}
void systemi2c_stop()
{
}




void systemi2c_init()
{
	fp=-1;
	where[0]=where[1]=where[2]=where[3]=-1;
}
void systemi2c_kill()
{
	if(fp>0)
	{
		close(fp);

		fp=-1;
		where[0]=where[1]=where[2]=where[3]=-1;
	}
}
