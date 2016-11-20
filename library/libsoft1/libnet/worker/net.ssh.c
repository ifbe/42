#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int cmp(u8*,u8*);
int ncmp(u8*,u8*,int);
int readserver(u64 fd, u8* addr, u64 offset, u64 count);
int writeserver(u64 fd, u8* addr, u64 offset, u64 count);
//
void printmemory(u8*, int);
void say(u8*, ...);




static int printalgorithm(u8* buf,int len)
{
	int j,k;
	u8 temp[64];

	j=4;
	k=0;
	while(1)
	{
		if( (buf[j] != ',') && (buf[j] != 0) )
		{
			temp[k] = buf[j];

			k++;
			if(k>60)
			{
				say("error@algorithm\n");
				break;
			}
		}
		else
		{
			temp[k]=0;
			say("	%s\n",temp);

			k=0;
		}

		//
		j++;
		if(j>len)break;
	}
}
static int secureshell_read_algorithm(u8* buf, u64 len)
{
	int j;
	int offset;
	u32 temp;

	//cookie
	for(j=0;j<16;j++)
	{
		say("%02x ", buf[6+j]);
	}
	say("\n");

	//
	offset = 0x16;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]key_exchange_algorithm\n", offset,temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]server_host_key_algorithm\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]encryption_algorithms_client_to_server\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]encryption_algorithms_server_to_client\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]mac_algorithms_client_to_server\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]mac_algorithms_server_to_client\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]compression_algorithms_client_to_server\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]compression_algorithms_server_to_client\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_client_to_server\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_server_to_client\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_server_to_client\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]first_kex_packet_follows\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

byebye:
	return 0x20;
}
static int secureshell_read(u8* buf, u64 len)
{
	int j;
	int offset;
	u32 temp;

	temp = buf[0];
	temp = (temp<<8) + buf[1];
	temp = (temp<<8) + buf[2];
	temp = (temp<<8) + buf[3];
	say("total=%x\n",temp);
	say("plen=%x\n", buf[4]);
	say("type=%x\n", buf[5]);
	if(buf[5] == 0x14)
	{
		secureshell_read_algorithm(buf,len);
	}
	else
	{
		printmemory(buf,temp);
	}

	return buf[5];
}




static int secureshell_write(u64 fd, u8* buf, int len)
{
	writeserver(fd, buf, 0, len);
}




//why,what,where,when
int serve_secureshell(u64* p, u8* buf, u64 len)
{
	int ret;
	if(p[1] == 0x20)
	{
		writeserver(p[2], "SSH-2.0-42\r\n", 0, 12);
		p[1] = 0x21;
	}
	else
	{
		ret = secureshell_read(buf, len);
		if(ret == 0x14)
		{
			secureshell_write(p[2], buf, len);
		}
	}

	//
	return 0x20;
}
