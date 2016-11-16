#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void sha1sum(u8* out, u8* in, int len);
void base64_encode(u8* out,u8* in, int len);
void datastr2hexstr(u8* out, u8* in, int len);
u32 getrandom();
//
int serverwrite(u64 fd, u8* addr, u64 offset, u64 count);
void epoll_del(u64 fd);
//
void say(char*, ...);




//
static u8* datahome;
//
static u8 buf1[256];
static u8 buf2[256];
static u8 fixed_salt[256];
static u8 temp_salt[256];




void serve_websocket(int fd, char* recvbuf)
{
/*
	int i,j,k;
	int type,masked;
	unsigned char key[4];
	u64 len;

	//fd
	say("[%d]",fd);

	//byte0.bit7
	if((recvbuf[0]&0x80)==0x80)say("tail,");
	else say("part,");

	//byte0.[3,0]
	k = recvbuf[0]&0xf;
	if(k==0)
	{
		type=0;
		say("external,");
	}
	else if(k==1)
	{
		type=1;
		say("text,");
	}
	else if(k==2)
	{
		type=2;
		say("binary,");
	}
	else if(k==9)
	{
		say("ping\n");
		return;
	}
	else if(k==0xa)
	{
		say("pong\n");
		return;
	}
	else if(k==8)
	{
		say("[%d]type8->close\n",fd);
		epoll_del(fd);
		return;
	}
	else
	{
		say("[%d]known->close\n",fd);
		epoll_del(fd);
		return;
	}

	//byte1.bit7
	if( (recvbuf[1]>>7) == 1)
	{
		masked=1;
		say("masked,");
	}
	else
	{
		masked=0;
		say("unmasked,");
	}

	//
	k = recvbuf[1]&0x7f;
	if(k==127)
	{
		len     = ((u64)recvbuf[2]<<56)
			+ ((u64)recvbuf[3]<<48)
			+ ((u64)recvbuf[4]<<40)
			+ ((u64)recvbuf[5]<<32)
			+ ((u64)recvbuf[6]<<24)
			+ ((u64)recvbuf[7]<<16)
			+ ((u64)recvbuf[8]<<8)
			+ recvbuf[9];
		k = 10;
		say("len=%llx,", len);
	}
	else if(k==126)
	{
		len     = (recvbuf[2]<<8)
			+ (recvbuf[3]);
		k = 4;
		say("len=%llx,", len);
	}
	else
	{
		len = k;
		k = 2;
		say("len=%llx,", len);
	}

	if(masked != 1)say("\n");
	else
	{
		*(u32*)key = *(u32*)(recvbuf + k);
		j = k;
		k += 4;
		say("key=%x\n",*(u32*)key);

		if(type==1)
		{
			recvbuf[0] &= 0x8f;
			recvbuf[1] &= 0x7f;
			for(i=0;i<len;i++)
			{
				datahome[i] = recvbuf[i+k] ^ key[i%4];
				//say("%c",recvbuf[j+i]);
			}
			datahome[len] = 0;
			//say("\n");
		}//type=ascii
	}//masked=1
*/




/*
reply:
	if(clienttype[fd]==0x10)
	{
		say("[%d][stage0][client]%s\n", fd, event_queue);

		//
		i = snprintf(recvbuf+2, 20, "4....2..");
		recvbuf[0] = 0x81;
		recvbuf[1] = i;
		i = write(fd, recvbuf, 2+i);

		//
		say("[%d][stage0][server]%s\n\n", fd, recvbuf+2);
		clienttype[fd] = 0x11;
	}

	else if(clienttype[fd]==0x11)
	{
		say("[%d][stage1][client]username=%s\n", fd, event_queue);
		if(0)
		{
			i = snprintf(recvbuf+2, 20, "fail");
			recvbuf[0] = 0x81;
			recvbuf[1] = i;
			i = write(fd, recvbuf, 2+i);

			epoll_del(fd);
			say("[%d]stage1 failed\n", fd);
			return;
		}

		else
		{
			//
			snprintf(fixed_salt, 256, "%08x", getrandom());
			snprintf( temp_salt, 256, "%08x", getrandom());
			i = snprintf(recvbuf+2, 20, "%s,%s", fixed_salt, temp_salt);
			recvbuf[0] = 0x81;
			recvbuf[1] = i;
			i = write(fd, recvbuf, 2+i);

			//
			say("[%d][stage1][server]challenge=%s\n\n", fd, recvbuf+2);
			clienttype[fd] = 0x12;
		}
	}
	else if(clienttype[fd]==0x12)
	{
		say("[%d][stage2][client]response=%s\n", fd, event_queue);

		//
		snprintf(buf2, 256, "%s%s", fixed_salt, "42");
		sha1sum(buf1, buf2, strlen(buf2));
		datastr2hexstr(buf2, buf1, 20);
		say("   %s\n",buf2);

		snprintf(buf1, 256, "%s%s", temp_salt, buf2);
		sha1sum(buf2, buf1, strlen(buf1));
		datastr2hexstr(buf1, buf2, 20);
		say("   %s\n",buf1);

		//if(strncmp(event_queue, buf1, 256) != 0)
		//{
			//epoll_del(fd);
			//say("[%d][stage2]incorrect\n", fd);
			//return;
		//}

		//write
		i = snprintf(recvbuf+2, 20, "correct");
		recvbuf[0] = 0x81;
		recvbuf[1] = i;
		i = write(fd, recvbuf, 2+i);

		//count
		websocket_count++;
		if(fd > websocket_last)websocket_last = fd;

		//type
		say("[%d][stage2][server]status=%s\n\n", fd, recvbuf+2);
		clienttype[fd] = 0x1f;

		//event
		*(u64*)(event_queue+0) = 0xabcdef;
		//eventwrite(*(u64*)(event_queue+8), *(u64*)event_queue, fd, 0);
	}
	else if(clienttype[fd]==0x1f)
	{
		say("[%d]:%s\n", fd, event_queue);

		len = *(u32*)event_queue;
		if(len == 0x2064626b)
		{
			len = atoi(event_queue+4);
			*(u64*)(event_queue+8) = len;
			*(u64*)(event_queue+0) = 0x64626b;
		}
		else if(len == 0x72616863)
		{
			len = atoi(event_queue+5);
			*(u64*)(event_queue+8) = len;
			*(u32*)(event_queue+4) = 0;
		}
		//eventwrite(*(u64*)(event_queue+8), *(u64*)event_queue, fd, 0);
	}
*/
}




static void websocket_write(char* sendbuf)
{
/*
	int fd;
	int ret;

	u8 type;
	u8* base;

	u64 headlen;
	u64 bodylen;

	//type
	type = 1;
	base = sendbuf + 0x1000;

	bodylen = strlen(base);

	//len
	if(bodylen<=125)
	{
		headlen = 2;
		*(base-2) = 0x80|type;
		*(base-1) = bodylen;
	}
	else if(bodylen<0xffff)
	{
		headlen = 4;
		*(base-4) = 0x80|type;
		*(base-3) = 126;
		*(base-2) = (bodylen>>8)&0xff;
		*(base-1) = bodylen&0xff;
	}
	else
	{
		headlen = 10;
		*(base-10)= 0x80|type;
		*(base-9) = 127;
		*(base-8) = (bodylen>>56)&0xff;
		*(base-7) = (bodylen>>48)&0xff;
		*(base-6) = (bodylen>>40)&0xff;
		*(base-5) = (bodylen>>32)&0xff;
		*(base-4) = (bodylen>>24)&0xff;
		*(base-3) = (bodylen>>16)&0xff;
		*(base-2) = (bodylen>>8)&0xff;
		*(base-1) = (bodylen)&0xff;
	}

	//write
	//printmemory(base-headlen, 0x200);
	ret = serverwrite(fd, base-headlen, 0, headlen+bodylen);
	if(ret > 0) say("=>%d\n",fd);
	else say("error@%d\n\n\n\n\n",fd);
*/
}
static void websocket_read()
{
}
static void websocket_list()
{
}
static void websocket_choose()
{
}
static void websocket_start(u64 type,char* p)
{
}
static void websocket_stop()
{
}
void websocket_create(char* softhome, u64* p)
{
	//
	datahome = softhome + 0x300000;

/*
	p[0]=0x74656e;		//type
	p[1]=0x7377;		//id

	p[10]=(u64)websocket_start;
	p[11]=(u64)websocket_stop;
	p[12]=(u64)websocket_list;
	p[13]=(u64)websocket_choose;
	p[14]=(u64)websocket_read;
	p[15]=(u64)websocket_write;
*/
}
void websocket_delete()
{
	websocket_stop();
}
