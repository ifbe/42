#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int readsocket(u64 fd, u8* addr, u64 offset, u64 count);
int writesocket(u64 fd, u8* addr, u64 offset, u64 count);
//
void generatePG(void*, int, void*, int);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int fmt(void*, int, void*, ...);
void printmemory(void*, int);
void say(void*, ...);




static void printalgorithm(u8* buf, int len)
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
	say("[6,15]cookie\n	");
	for(temp=0;temp<16;temp++)
	{
		say("%02x ", buf[6+temp]);
	}
	say("\n");
	offset = 0x16;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]key_exchange_algorithm\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]server_host_key_algorithm\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]encryption_algorithms_client_to_server\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]encryption_algorithms_server_to_client\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]mac_algorithms_client_to_server\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]mac_algorithms_server_to_client\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]compression_algorithms_client_to_server\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	say("[%x,%x]compression_algorithms_server_to_client\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_client_to_server\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_server_to_client\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]languages_server_to_client\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);
	offset += 4 + temp;

	//
	temp = buf[offset+2];
	temp = (temp<<8) + buf[offset+3];
	if(temp <= 0)goto byebye;
	say("[%x,%x]first_kex_packet_follows\n",
		offset, offset+temp-1);
	printalgorithm(buf+offset, temp+4);

byebye:
	return 0x20;
}
static int secureshell_read_0x1e(u8* buf, u64 len)
{
	u32 temp;
	int j = 6;

	temp = buf[j+2];
	temp = (temp<<8) + buf[j+3];
	printmemory(buf+j+4, temp);
	return temp;
}
static int secureshell_read_0x1f(u8* buf, u64 len)
{
	u32 Plen, Glen, Slen;
	int j=6;

	//P
	Plen = (buf[j+2]<<8) + buf[j+3];
	say("[%x,%x]P\n", j+4, j+Plen+3);
	printmemory(buf+j+4, Plen);
	j += 4 + Plen;

	//G
	Glen = (buf[j+2]<<8) + buf[j+3];
	say("[%x,%x]G\n", j+4, j+Glen+3);
	printmemory(buf+j+4, Glen);
	j += 4 + Glen;

	//sig
	Slen = (buf[j+2]<<8) + buf[j+3];
	say("[%x,%x]S\n", j+4, j+Slen+3);
	printmemory(buf+j+4, Slen);
	j += 4 + Slen;

	return j;
}
static int secureshell_read_0x22(u8* buf, u64 len)
{
	int min;
	int prefer;
	int max;

	min = (buf[8]<<8) + buf[9];
	prefer = (buf[0xc]<<8) + buf[0xd];
	max = (buf[0x10]<<8) + buf[0x11];

	say("[6,11]DH GEX\n");
	say("	min=%d\n", min);
	say("	prefer=%d\n", prefer);
	say("	max=%d\n", max);

	return prefer;
}




static int secureshell_write_head(u8* buf, int len)
{
	int j,temp;

	//padding
	if((len&7) < 4)temp = 8-(len&7);
	else temp = 16-(len&7);
	for(j=0;j<temp;j++)
	{
		buf[len+j] = 0;
	}
	len += temp;

	//head
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = ((len-4)>>8)&0xff;
	buf[3] = (len-4)&0xff;
	buf[4] = temp;

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
	temp = fmt(buf+offset+4, 999,
		"diffie-hellman-group-exchange-sha256,"
		"diffie-hellman-group-exchange-sha1"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//server_host_key_algorithm
	temp = fmt(buf+offset+4, 999,
		"ssh-rsa,"
		"rsa-sha2-256,"
		"rsa-sha2-512"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//encryption_algorithms_client_to_server
	temp = fmt(buf+offset+4, 999,
		"aes128-cbc,"
		"aes256-cbc,"
		"3des-cbc"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//encryption_algorithms_server_to_client
	temp = fmt(buf+offset+4, 999,
		"aes128-cbc,"
		"aes256-cbc,"
		"3des-cbc"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//mac_algorithms_client_to_server
	temp = fmt(buf+offset+4, 999,
		"hmac-sha1,"
		"hmac-sha2-256,"
		"hmac-sha2-512"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//mac_algorithms_server_to_client
	temp = fmt(buf+offset+4, 999,
		"hmac-sha1,"
		"hmac-sha2-256,"
		"hmac-sha2-512"
	);
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//compression_algorithms_client_to_server
	temp = fmt(buf+offset+4, 999, "none");
	buf[offset+0] = buf[offset+1] = 0;
	buf[offset+2] = (temp>>8)&0xff;
	buf[offset+3] = temp&0xff;
	offset += 4 + temp;

	//compression_algorithms_server_to_client
	temp = fmt(buf+offset+4, 999, "none");
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
	return offset;
}
static int secureshell_write_0x1f(u8* buf, u64 len)
{
	int offset;
	int Plen;
	int Glen;
	int Slen;
	u8* Pbuf;
	u8* Gbuf;
	u8* Sbuf;

	//
	buf[5] = 0x1f;
	offset = 6;

	//P.len, P.val
	Plen = 0x68;
	Pbuf = buf + offset + 4;
	buf[offset+0] = 0;
	buf[offset+1] = 0;
	buf[offset+2] = (Plen>>8)&0xff;
	buf[offset+3] = Plen&0xff;
	offset += 4 + Plen;

	//G.len, G.val
	Glen = 0x20;
	Gbuf = buf + offset + 4;
	buf[offset+0] = 0;
	buf[offset+1] = 0;
	buf[offset+2] = (Glen>>8)&0xff;
	buf[offset+3] = Glen&0xff;
	offset += 4 + Glen;

	//sig.len, sig.val
	Slen = 0x64;
	Sbuf = buf + offset + 4;
	buf[offset+0] = 0;
	buf[offset+1] = 0;
	buf[offset+2] = (Slen>>8)&0xff;
	buf[offset+3] = Slen&0xff;
	offset += 4 + Slen;

	//
	generatePG(Pbuf, Plen, Gbuf, Glen);
	offset = secureshell_write_head(buf, offset);

	//
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
	return offset;
}






//why,what,where,when
static u8 version[]="SSH-2.0-finalanswer_42\r\n";
static int secureshell_read(u8* buf, u64 len)
{
	int offset;
	u32 temp;

	temp = buf[0];
	temp = (temp<<8) + buf[1];
	temp = (temp<<8) + buf[2];
	temp = (temp<<8) + buf[3];
	if(buf[5] == 0x14)
	{
		say(
			"[0,5]SSH_MSG_KEXINIT\n"
			"	total=%x\n"
			"	plen=%x\n"
			"	type=%x\n",
			temp, buf[4], buf[5]
		);
		secureshell_read_0x14(buf,len);
	}
	else if(buf[5] == 0x1e)
	{
		say(
			"[0,5]SSH_MSG_KEX_DH_GEX_REQUEST_OLD\n"
			"	total=%x\n"
			"	plen=%x\n"
			"	type=%x\n",
			temp, buf[4], buf[5]
		);
		secureshell_read_0x1e(buf,len);
	}
	else if(buf[5] == 0x1f)
	{
		say(
			"[0,5]SSH_MSG_KEX_DH_GEX_GROUP\n"
			"	total=%x\n"
			"	plen=%x\n"
			"	type=%x\n",
			temp, buf[4], buf[5]
		);
		secureshell_read_0x1f(buf,len);
	}
	else if(buf[5] == 0x22)
	{
		say(
			"[0,5]SSH_MSG_KEX_DH_GEX_REQUEST\n"
			"	total=%x\n"
			"	plen=%x\n"
			"	type=%x\n",
			temp, buf[4], buf[5]
		);
		secureshell_read_0x22(buf,len);
	}
	else
	{
		printmemory(buf,temp);
	}

	say("\n\n\n\n");
	return buf[5];
}
int secureshell_write()
{
	return 0;
}








#define SSH 0x485353
#define ssh 0x687373
u64 serve_ssh(u64 fd, u64 type, u8* buf, u64 len)
{
	int ret = secureshell_read(buf, len);
	if(ret == 0x14)
	{
		//secureshell_write(buf, len);
		ret = secureshell_write_0x14(buf, len);
		writesocket(fd, buf, 0, ret);
	}
	else if(ret == 0x1e)
	{
		//try
		ret = secureshell_write_0x1f(buf, len);
		ret += secureshell_write_0x15(buf+ret, len);
		writesocket(fd, buf, 0, ret);
	}
	else if(ret == 0x22)
	{
		//try
		ret = secureshell_write_0x1f(buf, len);
		ret += secureshell_write_0x15(buf+ret, len);
		writesocket(fd, buf, 0, ret);
	}
	return SSH;
}
u64 check_ssh(u64 fd, u64 type, u8* buf, u64 len)
{
	if(ncmp(buf, "SSH-2.0-", 8) == 0)
	{
		writesocket(fd, version, 0, sizeof(version)-1);
		return SSH;
	}

	return 0;
}
