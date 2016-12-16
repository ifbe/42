#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void sha1sum(u8* out, u8* in, int len);
void base64_encode(u8* out,u8* in, int len);
void datastr2hexstr(u8* out, u8* in, int len);
int findzero(void* p);
int findhead(void* p);
int findtail(void* p);
u8* findstr(void* src, int max, void* target, int tarlen);
//
int readserver(u64 fd, u8* addr, u64 offset, u64 count);
int writeserver(u64 fd, u8* addr, u64 offset, u64 count);
u32 getrandom();
u64 gettime();
//
int diary(void*, int, void*, ...);
void say(void*, ...);




//
static u8 fixed_salt[256];
static u8 temp_salt[256];




static int websocket_read(u8* buf, int len)
{
	int i,j,k;
	int type,masked;
	unsigned char key[4];
	u64 count;

	//byte0.bit7
	if((buf[0]&0x80)==0x80)say("tail,");
	else say("part,");

	//byte0.[3,0]
	k = buf[0]&0xf;
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
		return 0;
	}
	else if(k==0xa)
	{
		say("pong\n");
		return 0;
	}
	else if(k==8)
	{
		//say("[%d]type8->close\n",fd);
		//epoll_del(fd);
		return -1;
	}
	else
	{
		//say("[%d]known->close\n",fd);
		//epoll_del(fd);
		return -2;
	}

	//byte1.bit7
	if( (buf[1]>>7) == 1)
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
	k = buf[1]&0x7f;
	if(k==127)
	{
		count	= ((u64)buf[2]<<56)
			+ ((u64)buf[3]<<48)
			+ ((u64)buf[4]<<40)
			+ ((u64)buf[5]<<32)
			+ ((u64)buf[6]<<24)
			+ ((u64)buf[7]<<16)
			+ ((u64)buf[8]<<8)
			+ buf[9];
		k = 10;
	}
	else if(k==126)
	{
		count	= (buf[2]<<8)
			+ (buf[3]);
		k = 4;
	}
	else
	{
		count = k;
		k = 2;
	}
	say("count=%llx,", count);

	if(masked != 1)say("\n");
	else
	{
		*(u32*)key = *(u32*)(buf + k);
		j = k;
		k += 4;
		say("key=%x\n",*(u32*)key);

		if(type==1)
		{
			buf[0] &= 0x8f;
			buf[1] &= 0x7f;
			for(i=0;i<count;i++)
			{
				buf[i] = buf[i+k] ^ key[i%4];
				//say("%c",buf[j+i]);
			}
			buf[count] = 0;
			//say("\n");
		}//type=ascii
	}//masked=1

	say("%s\n",buf);
	return count;
}
static void websocket_write(u64 fd, u8* buf, u64 len)
{
	u8 headbuf[16];
	int headlen;
	int ret;

	//len
	headbuf[0] = 0x81;
	if(len<=125)
	{
		headlen = 2;
		headbuf[1] = len;
	}
	else if(len<0xffff)
	{
		headlen = 4;
		headbuf[1] = 126;
		headbuf[2] = (len>>8)&0xff;
		headbuf[3] = len&0xff;
	}
	else
	{
		headlen = 10;
		headbuf[1] = 127;
		headbuf[2] = (len>>56)&0xff;
		headbuf[3] = (len>>48)&0xff;
		headbuf[4] = (len>>40)&0xff;
		headbuf[5] = (len>>32)&0xff;
		headbuf[6] = (len>>24)&0xff;
		headbuf[7] = (len>>16)&0xff;
		headbuf[8] = (len>>8)&0xff;
		headbuf[9] = (len)&0xff;
	}

	//write
	ret = writeserver(fd, headbuf, 0, headlen);
	ret = writeserver(fd, buf, 0, len);
}




int handshake_websocket(u64* p, u8* buf, int len)
{
        int j;
	u8* Sec_WebSocket_Key;
        u8 buf1[256];
        u8 buf2[256];
	say("%s(%d)\n",buf,len);

	//
	Sec_WebSocket_Key = findstr(buf, len, "Sec-WebSocket-Key", 17);
	if(Sec_WebSocket_Key == 0)return 0;
	Sec_WebSocket_Key += 19;

        //在Sec_WebSocket_Key尾巴上添加一个固定的字符串
        j = findtail(Sec_WebSocket_Key);
        j += diary(Sec_WebSocket_Key + j, 256,
		"258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
        say("Sec_WebSocket_Key=%s\n", Sec_WebSocket_Key);

        //对这个字符串做一次sha1
        sha1sum(buf1, Sec_WebSocket_Key, j);
	say("sha1=");
        for(j=0;j<20;j++)say("%.2x",buf1[j]);
        say("\n");

        //把sha1的结果以base64格式编码
        base64_encode(buf2 ,buf1, 20 );
        say("base64=%s\n",buf2);

        //把base64的结果作为accept密钥
        j = diary(buf1, 256,
                "HTTP/1.1 101 Switching Protocols\r\n"
                "Upgrade: websocket\r\n"
                "Connection: Upgrade\r\n"
                "Sec-WebSocket-Accept: %s\r\n\r\n",

                buf2
        );

        //发出去
        j = writeserver(p[2], buf1, 0, j);
        say("%s", buf1);

        //
	p[1] = 0x1f;
        return 0x1f;
}
int serve_ws(u64* p, u8* buf, u64 len)
{
	int ret;

	if(p[1] == 0x11)
	{
		handshake_websocket(p, buf, len);
	}
	else
	{
		//
		ret = websocket_read(buf, len);
		if(ret < 0)return ret;

		//
		websocket_write(p[2], (void*)"hahahaha", 8);
		return 0x1f;
	}

	return 0;
/*
	if(type==0x10)
	{
		say("[%d][stage0][client]%s\n", fd, event_queue);

		//
		i = snprintf(recvbuf+2, 20, "4....2..");
		recvbuf[0] = 0x81;
		recvbuf[1] = i;
		i = write(fd, recvbuf, 2+i);

		//
		say("[%d][stage0][server]%s\n\n", fd, recvbuf+2);
		type = 0x11;
	}

	else if(type==0x11)
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
			type = 0x12;
		}
	}
	else if(type==0x12)
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
		type = 0x1f;

		//event
		*(u64*)(event_queue+0) = 0xabcdef;
		//eventwrite(*(u64*)(event_queue+8), *(u64*)event_queue, fd, 0);
	}
	else if(type==0x1f)
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
int serve_wss(u64* p, u8* buf, u64 len)
{
	return 0;
}
