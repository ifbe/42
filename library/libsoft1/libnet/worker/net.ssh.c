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





static int ssh_read_algorithm(u8* buf, u64 len)
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
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]server_host_key_algorithm\n", offset, temp);
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]encryption_algorithms_client_to_server\n", offset, temp);
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]encryption_algorithms_server_to_client\n", offset, temp);
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]mac_algorithms_client_to_server\n", offset, temp);
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]mac_algorithms_server_to_client\n", offset, temp);
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]compression_algorithms_client_to_server\n", offset, temp);
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]compression_algorithms_server_to_client\n", offset, temp);
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_client_to_server\n", offset, temp);
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_server_to_client\n", offset, temp);
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_server_to_client\n", offset, temp);
	printmemory(buf+offset, temp+4);

	//
	offset += 4 + temp;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]first_kex_packet_follows\n", offset, temp);
	printmemory(buf+offset, temp+4);

byebye:
	return 0x20;
}
static int ssh_read(u8* buf, u64 len)
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
		ssh_read_algorithm(buf,len);
	}
	else
	{
		printmemory(buf,temp);
	}

	return buf[5];
}
static int ssh_write()
{
}
static void ssh_list()
{
}
static void ssh_choose()
{
}
static void ssh_start(u64 type,char* p)
{
}
static void ssh_stop()
{
}
void ssh_create(char* world,u64* p)
{
/*
	p[0]=0x74656e;		//type
	p[1]=0x687373;		//id

	p[10]=(u64)ssh_start;
	p[11]=(u64)ssh_stop;
	p[12]=(u64)ssh_list;
	p[13]=(u64)ssh_choose;
	p[14]=(u64)ssh_read;
	p[15]=(u64)ssh_write;
*/
}
void ssh_delete()
{
	ssh_stop();
}




int serve_secureshell(u64 fd, u8* buf, u64 len)
{
	int ret = ssh_read(buf, len);
	if(ret == 0x14)writeserver(fd, buf, 0, len);
	return 0x20;
}
