#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void say(void*, ...);




int jpg_check(u8* buf)
{
	if(buf[0] != 0xff)return 0;
	if(buf[1] != 0xd8)return 0;
	if(buf[2] != 0xff)return 0;
	if(buf[3] < 0xc0)return 0;
	return 1;
}
int jpg_explain(u8* buf, int len)
{
	int j,k,m,n;
	int type,length;

	j = 2;
	while(1)
	{
		if(j>=len)break;
		if(buf[j] != 0xff)break;

		type = buf[j+1];
		length = (buf[j+2]<<8) + buf[j+3];
		if(type == 0xc0)
		{
			say("[%x,%x]SOF0\n", j, j+length+1, type);
			say("	acc:%d\n", buf[j+4]);
			say("	height:%d\n", (buf[j+5]<<8) + buf[j+6]);
			say("	width:%d\n", (buf[j+7]<<8) + buf[j+8]);
			say("	%x,%x\n", buf[j+11], buf[j+12]);
			say("	%x,%x\n", buf[j+14], buf[j+15]);
			say("	%x,%x\n", buf[j+17], buf[j+18]);
		}
		else if(type == 0xc4)
		{
			say("[%x,%x]DHT\n", j, j+length+1, type);
			//printmemory(buf+j+4, length-2);

			k=j+4;
			n=0;
			for(m=k+1;m<k+0x11;m++)n+=buf[m];
			say("	[%x,%x]%02x\n", k, k+n+16,buf[k]);

			k+=16+n+1;
			n=0;
			for(m=k+1;m<k+0x11;m++)n+=buf[m];
			say("	[%x,%x]%02x\n", k, k+n+16,buf[k]);

			k+=16+n+1;
			n=0;
			for(m=k+1;m<k+0x11;m++)n+=buf[m];
			say("	[%x,%x]%02x\n", k, k+n+16,buf[k]);

			k+=16+n+1;
			n=0;
			for(m=k+1;m<k+0x11;m++)n+=buf[m];
			say("	[%x,%x]%02x\n", k, k+n+16,buf[k]);
		}
		else if(type == 0xda)
		{
			say("[%x,%x]SOS\n", j, j+length+1, type);
			say("	Y:%x	Cr:%x	Cb:%x\n",
				buf[j+6],
				buf[j+8],
				buf[j+10]
			);
		}
		else if(type == 0xdb)
		{
			say("[%x,%x]DQT\n", j, j+length+1, type);
			//printmemory(buf+j+4, length-2);

			k = j+4;
			while(k < j+length)
			{
				m = (1 + ((buf[k]>>4)&0xf)) << 6;
				n = buf[k]&0xf;
				say("	[%x,%x]%x\n",k, k+m, n);
				k += 1 + m;
			}
		}
		else if(type == 0xdd)
		{
			say("[%x,%x]DRI\n", j, j+length+1, type);
			say("	%x\n", (buf[j+4]<<8) + buf[j+5]);
		}
		else say("[%x,%x]%x\n", j, j+length+1, type);

		j += length + 2;
	}

	say("[%x,?]data\n", j);
	return 0;
}
