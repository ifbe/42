#include "artery.h"
//
void generatePG(void*, int, void*, int);
int ncmp(void*, void*, int);
int cmp(void*, void*);




static u8 version[]="SSH-2.0-finalanswer_42\r\n";




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
static int secureshell_read_0x14(u8* buf, int len)
{
	u32 j;
	int off;

	//cookie
	say("[6,15]cookie\n	");
	for(j=0;j<16;j++)say("%02x ", buf[6+j]);
	say("\n");
	off = 0x16;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	say("[%x,%x]key_exchange_algorithm\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	say("[%x,%x]server_host_key_algorithm\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	say("[%x,%x]enc_alg_c2s\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	say("[%x,%x]enc_alg_s2c\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	say("[%x,%x]mac_alg_c2s\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	say("[%x,%x]mac_alg_s2c\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	say("[%x,%x]comp_alg_c2s\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	say("[%x,%x]comp_alg_s2c\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	if(j <= 0)goto byebye;
	say("[%x,%x]lang_c2s\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	if(j <= 0)goto byebye;
	say("[%x,%x]lang_s2c\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	if(j <= 0)goto byebye;
	say("[%x,%x]first_kex\n", off, off+j-1);
	printalgorithm(buf+off, j+4);

byebye:
	return 0x14;
}
static int secureshell_read_0x1e(u8* buf, int len)
{
	u32 j;
	int off = 6;

	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	printmemory(buf+off+4, j);
	return j;
}
static int secureshell_read_0x1f(u8* buf, int len)
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
static int secureshell_read_0x22(u8* buf, int len)
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





int secureshell_write_handshake(u8* buf, int len)
{
	int j;
	len = sizeof(version)-1;
	for(j=0;j<len;j++)buf[j] = version[j];
	return len;
}
int secureshell_write_head(u8* buf, int len)
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
static int secureshell_write_0x14(u8* buf, int len)
{
	int j;
	int off;

	//cookie unchanged
	for(j=0;j<16;j++)buf[off+j] = j;
	off = 0x16;

	//key_exchange_algorithm
	j = mysnprintf(buf+off+4, 999,
		"diffie-hellman-group-exchange-sha256,"
		"diffie-hellman-group-exchange-sha1"
	);
	buf[off+0] = buf[off+1] = 0;
	buf[off+2] = (j>>8)&0xff;
	buf[off+3] = j&0xff;
	off += 4 + j;

	//server_host_key_algorithm
	j = mysnprintf(buf+off+4, 999,
		"ssh-rsa,"
		"rsa-sha2-256,"
		"rsa-sha2-512"
	);
	buf[off+0] = buf[off+1] = 0;
	buf[off+2] = (j>>8)&0xff;
	buf[off+3] = j&0xff;
	off += 4 + j;

	//encryption_algorithms_client_to_server
	j = mysnprintf(buf+off+4, 999,
		"aes128-cbc,"
		"aes256-cbc,"
		"3des-cbc"
	);
	buf[off+0] = buf[off+1] = 0;
	buf[off+2] = (j>>8)&0xff;
	buf[off+3] = j&0xff;
	off += 4 + j;

	//encryption_algorithms_server_to_client
	j = mysnprintf(buf+off+4, 999,
		"aes128-cbc,"
		"aes256-cbc,"
		"3des-cbc"
	);
	buf[off+0] = buf[off+1] = 0;
	buf[off+2] = (j>>8)&0xff;
	buf[off+3] = j&0xff;
	off += 4 + j;

	//mac_algorithms_client_to_server
	j = mysnprintf(buf+off+4, 999,
		"hmac-sha1,"
		"hmac-sha2-256,"
		"hmac-sha2-512"
	);
	buf[off+0] = buf[off+1] = 0;
	buf[off+2] = (j>>8)&0xff;
	buf[off+3] = j&0xff;
	off += 4 + j;

	//mac_algorithms_server_to_client
	j = mysnprintf(buf+off+4, 999,
		"hmac-sha1,"
		"hmac-sha2-256,"
		"hmac-sha2-512"
	);
	buf[off+0] = buf[off+1] = 0;
	buf[off+2] = (j>>8)&0xff;
	buf[off+3] = j&0xff;
	off += 4 + j;

	//compression_algorithms_client_to_server
	j = mysnprintf(buf+off+4, 999, "none");
	buf[off+0] = buf[off+1] = 0;
	buf[off+2] = (j>>8)&0xff;
	buf[off+3] = j&0xff;
	off += 4 + j;

	//compression_algorithms_server_to_client
	j = mysnprintf(buf+off+4, 999, "none");
	buf[off+0] = buf[off+1] = 0;
	buf[off+2] = (j>>8)&0xff;
	buf[off+3] = j&0xff;
	off += 4 + j;

	//other
	for(j=off;j<off+32;j++)buf[j] = 0;
	off += 4+4+5;

	//
	buf[5] = 0x14;
	return secureshell_write_head(buf, off);
}
static int secureshell_write_0x1e(u8* buf, int len)
{
	int ret;
	int off=6;

	buf[off+0] = 0;
	buf[off+1] = 0;
	buf[off+2] = 0;
	buf[off+3] = 0x20;
	for(ret=0;ret<0x20;ret++)
	{
		buf[off+4+ret] = ret;
	}
	off += 4+0x20;

	buf[5] = 0x1e;
	return secureshell_write_head(buf, off);
}
static int secureshell_write_0x1f(u8* buf, int len)
{
	u8* Pbuf;
	u8* Gbuf;
	u8* Sbuf;
	int Plen;
	int Glen;
	int Slen;
	int off=6;

	//P.len, P.val
	Plen = 0x68;
	Pbuf = buf + off + 4;
	buf[off+0] = 0;
	buf[off+1] = 0;
	buf[off+2] = (Plen>>8)&0xff;
	buf[off+3] = Plen&0xff;
	off += 4 + Plen;

	//G.len, G.val
	Glen = 0x20;
	Gbuf = buf + off + 4;
	buf[off+0] = 0;
	buf[off+1] = 0;
	buf[off+2] = (Glen>>8)&0xff;
	buf[off+3] = Glen&0xff;
	off += 4 + Glen;

	//sig.len, sig.val
	Slen = 0x64;
	Sbuf = buf + off + 4;
	buf[off+0] = 0;
	buf[off+1] = 0;
	buf[off+2] = (Slen>>8)&0xff;
	buf[off+3] = Slen&0xff;
	off += 4 + Slen;

	//
	generatePG(Pbuf, Plen, Gbuf, Glen);

	//
	buf[5] = 0x1f;
	return secureshell_write_head(buf, off);
}
static int secureshell_write_0x15(u8* buf, int len)
{
	int j;
	int off=6;

	//
	for(j=off+4;j<off+4+10;j++)
	{
		buf[j] = j;
	}
	buf[off+0] = 0;
	buf[off+1] = 0;
	buf[off+2] = 0;
	buf[off+3] = 10;
	off += 4 + 10;

	//
	for(j=off;j<off+0x54;j++)
	{
		buf[j] = j;
	}
	off += 0x54;

	//
	buf[5] = 0x15;
	return secureshell_write_head(buf, off);
}






//why,what,where,when
static int secureshell_read(u8* buf, int len)
{
	u32 j;
	int off;

	j = buf[0];
	j = (j<<8) + buf[1];
	j = (j<<8) + buf[2];
	j = (j<<8) + buf[3];
	if(buf[5] == 0x14)
	{
		say(
			"[0,5]SSH_MSG_KEXINIT\n"
			"	total=%x\n"
			"	plen=%x\n"
			"	type=%x\n",
			j, buf[4], buf[5]
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
			j, buf[4], buf[5]
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
			j, buf[4], buf[5]
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
			j, buf[4], buf[5]
		);
		secureshell_read_0x22(buf,len);
	}
	else
	{
		printmemory(buf,j);
	}

	say("\n\n\n\n");
	return buf[5];
}
void ssh_start()
{
}
void ssh_stop()
{
}








#define SSH 0x485353
#define ssh 0x687373
int ssh_client(struct object* obj, int fd, u8* buf, int len)
{
	int ret;
	if(ncmp(buf, "SSH-2.0-", 8) == 0)
	{
		for(ret=0;ret<len;ret++)
		{
			if( (buf[ret] == 0xd) && (buf[ret+1] == 0xa) )
			{
				buf[ret] = buf[ret+1] = 0;
				say("%s\n",buf);

				break;
			}
		}
		if(ret+2 >= len)return ssh;

		buf += ret+2;
		len -= ret+2;
	}

	ret = secureshell_read(buf, len);
	if(ret == 0x14)
	{
		//protocol
		ret = secureshell_write_0x14(buf, len);
		writesocket(fd, 0, buf, ret);

		//keyexch
		ret = secureshell_write_0x1e(buf, len);
		writesocket(fd, 0, buf, ret);
	}
	else printmemory(buf,len);

	return ssh;
}
int ssh_server(struct object* obj, int fd, u8* buf, int len)
{
	int ret = secureshell_read(buf, len);
	if(ret == 0x14)
	{
		//secureshell_write(buf, len);
		ret = secureshell_write_0x14(buf, len);
		writesocket(fd, 0, buf, ret);
	}
	else if(ret == 0x1e)
	{
		//try
		ret = secureshell_write_0x1f(buf, len);
		ret += secureshell_write_0x15(buf+ret, len);
		writesocket(fd, 0, buf, ret);
	}
	else if(ret == 0x22)
	{
		//try
		ret = secureshell_write_0x1f(buf, len);
		ret += secureshell_write_0x15(buf+ret, len);
		writesocket(fd, 0, buf, ret);
	}
	else printmemory(buf,len);

	return SSH;
}
int ssh_check(void* p, int fd, u8* buf, int len)
{
	if(ncmp(buf, "SSH-2.0-", 8) == 0)
	{
		writesocket(fd, 0, version, sizeof(version)-1);
		return SSH;
	}

	return 0;
}
