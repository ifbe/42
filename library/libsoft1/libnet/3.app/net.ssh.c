#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int cmp(u8*,u8*);
int ncmp(u8*,u8*,int);
int readserver(u64 fd, u8* addr, u64 offset, u64 count);
int writeserver(u64 fd, u8* addr, u64 offset, u64 count);
//
int diary(u8*, int, u8*, ...);
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
static int secureshell_read_0x14(u8* buf, u64 len)
{
	int offset;
	u32 temp;

	//cookie
	for(temp=0;temp<16;temp++)
	{
		say("%02x ", buf[6+temp]);
	}
	say("\n");
	offset = 0x16;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]key_exchange_algorithm\n", offset,temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]server_host_key_algorithm\n", offset, temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]encryption_algorithms_client_to_server\n", offset, temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]encryption_algorithms_server_to_client\n", offset, temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]mac_algorithms_client_to_server\n", offset, temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]mac_algorithms_server_to_client\n", offset, temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]compression_algorithms_client_to_server\n", offset, temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]compression_algorithms_server_to_client\n", offset, temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_client_to_server\n", offset, temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_server_to_client\n", offset, temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_server_to_client\n", offset, temp);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]first_kex_packet_follows\n", offset, temp);
	printalgorithm(buf+offset, temp+4);

byebye:
	return 0x20;
}
static int secureshell_read_0x1e(u8* buf, u64 len)
{
	int offset;
	u32 temp;

	offset = 0x6;
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	printmemory(buf+offset, temp+4);
}
static int secureshell_read(u8* buf, u64 len)
{
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
		secureshell_read_0x14(buf,len);
	}
	else if(buf[5] == 0x1e)
	{
		secureshell_read_0x1e(buf,len);
	}
	else
	{
		printmemory(buf,temp);
	}

	return buf[5];
}




static int secureshell_write_head(u8* buf, int len)
{
say("len(old)=%d\n",len);
	//padding
	int temp;
	if((len&7) < 4)temp = 8-(len&7);
	else temp = 16-(len&7);
	len += temp;

	//head
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = ((len-4)>>8)&0xff;
	buf[3] = (len-4)&0xff;
	buf[4] = temp;

say("len(new)=%d\n",len);
	return len;
}
static int secureshell_write_0x14(u8* buf, u64 len)
{
	int offset;
	int temp;

	//cookie unchanged
	for(temp=0;temp<16;temp++)buf[6+temp] = temp;
	offset = 0x16;

	//key_exchange_algorithm
	temp = diary(buf+offset+4, 999,
		"curve25519-sha256@libssh.org,"
		"diffie-hellman-group14-sha1,"
		"diffie-hellman-group-exchange-sha1,"
		"diffie-hellman-group-exchange-sha256"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//server_host_key_algorithm
	temp = diary(buf+offset+4, 999,
		"ssh-rsa,"
		"rsa-sha2-256,"
		"rsa-sha2-512"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//encryption_algorithms_client_to_server
	temp = diary(buf+offset+4, 999,
		"aes128-cbc,"
		"aes256-cbc,"
		"3des-cbc"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//encryption_algorithms_server_to_client
	temp = diary(buf+offset+4, 999,
		"aes128-cbc,"
		"aes256-cbc,"
		"3des-cbc"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//mac_algorithms_client_to_server
	temp = diary(buf+offset+4, 999,
		"hmac-sha1,"
		"hmac-sha2-256,"
		"hmac-sha2-512"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//mac_algorithms_server_to_client
	temp = diary(buf+offset+4, 999,
		"hmac-sha1,"
		"hmac-sha2-256,"
		"hmac-sha2-512"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//compression_algorithms_client_to_server
	temp = diary(buf+offset+4, 999, "none");
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//compression_algorithms_server_to_client
	temp = diary(buf+offset+4, 999, "none");
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//other
	for(temp=offset;temp<offset+32;temp++)buf[temp] = 0;
	offset += 4+4+5;

	//
	buf[5] = 0x14;
	offset = secureshell_write_head(buf, offset);

	//
	secureshell_read(buf, offset);
	return offset;
}
static int secureshell_write_0x1f(u8* buf, u64 len)
{
	int offset;
	int temp;

	//
	buf[5] = 0x1f;
	offset = 6;

	//KEX DH HOST KEY
	for(temp=offset+4;temp<offset+4+0x68;temp++)
	{
		buf[temp] = temp;
	}
	buf[offset+0] = 0;
	buf[offset+1] = 0;
	buf[offset+2] = 0;
	buf[offset+3] = 0x68;
	offset += 4 + 0x68;

	//MULTI PRECISION INTEGER
	for(temp=offset+4;temp<offset+4+0x20;temp++)
	{
		buf[temp] = temp;
	}
	buf[offset+0] = 0;
	buf[offset+1] = 0;
	buf[offset+2] = 0;
	buf[offset+3] = 0x20;
	offset += 4 + 0x20;

	//KEX DH SIGNATURE
	for(temp=offset+4;temp<offset+4+0x64;temp++)
	{
		buf[temp] = temp;
	}
	buf[offset+0] = 0;
	buf[offset+1] = 0;
	buf[offset+2] = 0;
	buf[offset+3] = 0x64;
	offset += 4 + 0x64;

	//
	for(temp=offset+4;temp<offset+4+0x64;temp++)
	{
		buf[temp] = temp;
	}
	buf[offset+0] = 0;
	buf[offset+1] = 0;
	buf[offset+2] = 0;
	buf[offset+3] = 0x64;
	offset += 4 + 0x64;

	//
	offset = secureshell_write_head(buf, offset);

	//
	//secureshell_read_0x1f(buf, offset);
	return offset;
}
static int secureshell_write_0x15(u8* buf, u64 len)
{
	int offset;
	int temp;

	//
	buf[5] = 0x15;
	offset = 6;

	//
	for(temp=offset+4;temp<offset+4+10;temp++)
	{
		buf[temp] = temp;
	}
	buf[offset+0] = 0;
	buf[offset+1] = 0;
	buf[offset+2] = 0;
	buf[offset+3] = 10;
	offset += 4 + 10;

	//
	for(temp=offset;temp<offset+0x54;temp++)
	{
		buf[temp] = temp;
	}
	offset += 0x54;

	//
	offset = secureshell_write_head(buf, offset);

	//
	//secureshell_read_0x1f(buf, offset);
	return offset;
}






//why,what,where,when
static u8 version[]="SSH-2.0-finalanswer_42\r\n";
int serve_secureshell(u64* p, u8* buf, u64 len)
{
	int ret;
	if(p[1] == 0x30)
	{
		writeserver(p[2], version, 0, sizeof(version)-1);
		p[1] = 0x31;
	}
	else
	{
		ret = secureshell_read(buf, len);
		if(ret == 0x14)
		{
			//secureshell_write(buf, len);
			ret = secureshell_write_0x14(buf, len);
			writeserver(p[2], buf, 0, ret);
		}
		else if(ret == 0x1e)
		{
			//try
			ret = secureshell_write_0x1f(buf, len);
			ret += secureshell_write_0x15(buf+ret, len);
			writeserver(p[2], buf, 0, ret);
		}
	}

	//
	return 0x30;
}
