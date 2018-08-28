#include "libsoft.h"
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





int secureshell_read_head(u8* buf, int len)
{
	return 0;
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
int secureshell_read_0x14(u8* buf, int len, u8* dst, int cnt)
{
	u32 j, off;
	if(0x14 != buf[5])return 0;

	j = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
	say(
		"[0,5]SSH_MSG_KEXINIT\n"
		"	total=%x\n"
		"	plen=%x\n"
		"	type=%x\n",
		j, buf[4], buf[5]
	);

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
int secureshell_write_0x14(u8* buf, int len, u8* dst, int cnt)
{
	int j;
	int off;

	//cookie unchanged
	for(j=0;j<16;j++)dst[off+j] = j;
	off = 0x16;

	//key_exchange_algorithm
	j = mysnprintf(dst+off+4, 999,
		"diffie-hellman-group-exchange-sha256,"
		"diffie-hellman-group-exchange-sha1"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//server_host_key_algorithm
	j = mysnprintf(dst+off+4, 999,
		"ssh-rsa,"
		"rsa-sha2-256,"
		"rsa-sha2-512"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//encryption_algorithms_client_to_server
	j = mysnprintf(dst+off+4, 999,
		"aes128-cbc,"
		"aes256-cbc,"
		"3des-cbc"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//encryption_algorithms_server_to_client
	j = mysnprintf(dst+off+4, 999,
		"aes128-cbc,"
		"aes256-cbc,"
		"3des-cbc"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//mac_algorithms_client_to_server
	j = mysnprintf(dst+off+4, 999,
		"hmac-sha1,"
		"hmac-sha2-256,"
		"hmac-sha2-512"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//mac_algorithms_server_to_client
	j = mysnprintf(dst+off+4, 999,
		"hmac-sha1,"
		"hmac-sha2-256,"
		"hmac-sha2-512"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//compression_algorithms_client_to_server
	j = mysnprintf(dst+off+4, 999, "none");
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//compression_algorithms_server_to_client
	j = mysnprintf(dst+off+4, 999, "none");
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//other
	for(j=off;j<off+32;j++)dst[j] = 0;
	off += 4+4+5;

	//
	dst[5] = 0x14;
	return secureshell_write_head(dst, off);
}




int secureshell_clientread_handshake(u8* buf, int len, u8* dst, int cnt)
{
	if(ncmp(buf, "SSH-2.0-", 8) == 0)
	{
		return mysnprintf(dst, cnt, version);
	}
	return 0;
}
int secureshell_clientwrite_handshake(u8* buf, int len, u8* dst, int cnt)
{
	return mysnprintf(dst, cnt, version);
}
int secureshell_clientread_handshake0x14(u8* buf, int len, u8* dst, int cnt)
{
	return secureshell_read_0x14(buf, len, dst, cnt);
}
int secureshell_clientwrite_handshake0x14(u8* buf, int len, u8* dst, int cnt)
{
	return secureshell_write_0x14(buf, len, dst, cnt);
}
int secureshell_clientwrite_handshake0x1e(u8* buf, int len, u8* dst, int cnt)
{
	int ret;
	int off=6;

	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = 0;
	dst[off+3] = 0x20;
	for(ret=0;ret<0x20;ret++)
	{
		dst[off+4+ret] = ret;
	}
	off += 4+0x20;

	dst[5] = 0x1e;
	return secureshell_write_head(dst, off);
}
int sshclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int ret;
	u8 tmp[0x1000];
	if(0 == ele->stage1)
	{
		secureshell_clientread_handshake(buf, len, tmp, 0x1000);

		ret = secureshell_clientwrite_handshake0x14(buf, len, tmp, 0x1000);
		if(ret)systemwrite(obj, pin, ele, sty, tmp, ret);
	}
	else if(1 == ele->stage1)
	{
		ret = secureshell_clientread_handshake0x14(buf, len, tmp, 0x1000);
	}
	else printmemory(buf, len);

	ele->stage1 += 1;
	return 0;
}
int sshclient_read()
{
	return 0;
}
int sshclient_delete(struct element* ele)
{
	return 0;
}
int sshclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	void* obj = systemcreate(_tcp_, url);
	if(0 == obj)return 0;

	ret = secureshell_clientwrite_handshake(url, 0, buf, 0x100);

	ret = systemwrite(obj, 0, ele, 0, buf, ret);
	if(ret <= 0)return 0;

	ele->type = _ssh_;
	ele->stage1 = 0;
	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}




int secureshell_serverread_handshake0x14(u8* buf, int len, u8* dst, int cnt)
{
	return secureshell_read_0x14(buf, len, dst, cnt);
}
int secureshell_serverwrite_handshake0x14(u8* buf, int len, u8* dst, int cnt)
{
	return secureshell_write_0x14(buf, len, dst, cnt);
}
int secureshell_serverread_handshake0x22(u8* buf, int len, u8* dst, int cnt)
{
	int min,max,prefer;
	u32 j, off;
	if(0x14 != buf[5])return 0;

	j = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
	say(
		"[0,5]SSH_MSG_KEXINIT\n"
		"	total=%x\n"
		"	plen=%x\n"
		"	type=%x\n",
		j, buf[4], buf[5]
	);

	min = (buf[8]<<8) + buf[9];
	max = (buf[0x10]<<8) + buf[0x11];
	prefer = (buf[0xc]<<8) + buf[0xd];

	say("[6,11]DH GEX\n");
	say("	min=%d\n", min);
	say("	max=%d\n", max);
	say("	prefer=%d\n", prefer);

	return 0;
}
int secureshell_serverwrite_handshake0x1f(u8* buf, int len, u8* dst, int cnt)
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
	Pbuf = dst + off + 4;
	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = (Plen>>8)&0xff;
	dst[off+3] = Plen&0xff;
	off += 4 + Plen;

	//G.len, G.val
	Glen = 0x20;
	Gbuf = dst + off + 4;
	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = (Glen>>8)&0xff;
	dst[off+3] = Glen&0xff;
	off += 4 + Glen;

	//sig.len, sig.val
	Slen = 0x64;
	Sbuf = dst + off + 4;
	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = (Slen>>8)&0xff;
	dst[off+3] = Slen&0xff;
	off += 4 + Slen;

	//
	generatePG(Pbuf, Plen, Gbuf, Glen);

	//
	dst[5] = 0x1f;
	return secureshell_write_head(dst, off);
}
int secureshell_serverwrite_handshake0x15(u8* buf, int len, u8* dst, int cnt)
{
	int j;
	int off=6;

	//
	for(j=off+4;j<off+4+10;j++)dst[j] = j;
	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = 0;
	dst[off+3] = 10;
	off += 4 + 10;

	//
	for(j=off;j<off+0x54;j++)dst[j] = j;
	off += 0x54;

	//
	dst[5] = 0x15;
	return secureshell_write_head(dst, off);
}
int sshserver_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int ret;
	u8 tmp[0x1000];

	if(0 == ele->stage1)
	{
		ret = secureshell_serverread_handshake0x14(buf, len, tmp, 0x100);

		ret = secureshell_serverwrite_handshake0x14(buf, len, tmp, 0x1000);
		if(ret)systemwrite(obj, pin, ele, sty, tmp, ret);
	}
	else if(1 == ele->stage1)
	{
		ret = secureshell_serverread_handshake0x22(buf, len, tmp, 0x100);

		ret = secureshell_serverwrite_handshake0x1f(buf, len, tmp, 0x1000);
		if(ret)ret += secureshell_serverwrite_handshake0x15(buf, len, tmp+ret, 0x1000-ret);
		if(ret)systemwrite(obj, pin, ele, sty, tmp, ret);
	}
	else printmemory(buf, len);

	ele->stage1 += 1;
	return 0;
}
int sshserver_read()
{
	return 0;
}
int sshserver_delete(struct element* ele)
{
	return 0;
}
int sshserver_create(struct element* ele, u8* url, u8* buf, int len)
{
	return 0;
}




int secureshell_serverread_handshake(u8* buf, int len, u8* dst, int cnt)
{
	if(ncmp(buf, "SSH-2.0-", 8) == 0)
	{
		return mysnprintf(dst, cnt, version);
	}
	return 0;
}
int secureshell_serverwrite_handshake(u8* buf, int len, u8* dst, int cnt)
{
	return 0;
}
int sshmaster_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int ret;
	u8 tmp[0x1000];
	struct element* e;

	ret = secureshell_serverread_handshake(buf, len, tmp, 0x100);
	if(ret)systemwrite(obj, pin, ele, sty, tmp, ret);

	e = arterycreate(_Ssh_, 0);
	if(e)relationcreate(e, 0, _art_, obj, 0, _fd_);
	return 0;
}
int sshmaster_read()
{
	return 0;
}
int sshmaster_delete(struct element* ele)
{
	return 0;
}
int sshmaster_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	void* obj = systemcreate(_TCP_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}
