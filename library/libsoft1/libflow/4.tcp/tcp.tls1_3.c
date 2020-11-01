#include "libsoft.h"




static u8 clientcipher[34] = {
0xba, 0xba,		//reserved GREASE
0x13, 0x01,		//TLS_AES_128_GCM_SHA256
0x13, 0x02,		//TLS_AES_256_GCM_SHA384
0x13, 0x03,		//TLS_CHACHA20_POLY1305_SHA256
0xc0, 0x2b,		//TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
0xc0, 0x2c,		//TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
0xc0, 0x2f,		//TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
0xc0, 0x30,		//TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384
0xcc, 0xa9,		//TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256
0xcc, 0xa8,		//TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256
0xc0, 0x13,		//TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA
0xc0, 0x14,		//TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA
0x00, 0x9c,		//TLS_RSA_WITH_AES_128_GCM_SHA256
0x00, 0x9d,		//TLS_RSA_WITH_AES_256_GCM_SHA384
0x00, 0x2f,		//TLS_RSA_WITH_AES_128_CBC_SHA
0x00, 0x35,		//TLS_RSA_WITH_AES_256_CBC_SHA
0x00, 0x0a		//TLS_RSA_WITH_3DES_EDE_CBC_SHA
};




//0000 = server name
int tls1v3_clienthello_extension_0000(u8* dst)
{
	int j;
	u8* tmp = dst+6;

	j = mysnprintf(tmp+3, 30, "www.deepstack.tech");
	tmp[0] = 0;
	tmp[1] = j >> 8;
	tmp[2] = j & 0xff;
	j += 3;

	dst[4] = j >> 8;
	dst[5] = j & 0xff;
	j += 2;

	dst[0] = 0x00;
	dst[1] = 0x00;
	dst[2] = j >> 8;
	dst[3] = j & 0xff;
	return j+4;
}
//0005 = status request
int tls1v3_clienthello_extension_0005(u8* dst)
{
	int j;
	u8* tmp = dst+4;

	tmp[0] = 0x01;	//OCSP
	tmp[1] = 0x00;
	tmp[2] = 0x00;
	tmp[3] = 0x00;
	tmp[4] = 0x00;
	j = 5;

	//this type
	dst[0] = 0x00;
	dst[1] = 0x05;
	dst[2] = j >> 8;
	dst[3] = j & 0xff;
	return j+4;
}
//000a = support groups
int tls1v3_clienthello_extension_000a(u8* dst)
{
	int j;
	u8* tmp = dst+6;
	tmp[0] = 0xaa;		//GREASE
	tmp[1] = 0xaa;
	tmp[2] = 0x00;		//x25519
	tmp[3] = 0x1d;
	tmp[4] = 0x00;		//secp256r1
	tmp[5] = 0x17;
	tmp[6] = 0x00;		//secp384r1
	tmp[7] = 0x18;
	j = 8;

	dst[4] = j >> 8;
	dst[5] = j & 0xff;
	j += 2;

	dst[0] = 0x00;
	dst[1] = 0x0a;
	dst[2] = j >> 8;
	dst[3] = j & 0xff;
	return j+4;
}
//000b = ec point format
int tls1v3_clienthello_extension_000b(u8* dst)
{
	dst[0] = 0x00;
	dst[1] = 0x0b;
	dst[2] = 0;
	dst[3] = 2;
	dst[4] = 1;
	dst[5] = 0;
	return 6;
}
//000d = signature algorithm
int tls1v3_clienthello_extension_000d(u8* dst)
{
	int j;
	u8* tmp = dst+6;

	tmp[ 0] = 0x04;		//ecdsa_secp256r1_sha256
	tmp[ 1] = 0x03;
	tmp[ 2] = 0x08;		//rsa_pss_rsae_sha256
	tmp[ 3] = 0x04;
	tmp[ 4] = 0x04;		//rsa_pkcs1_sha256
	tmp[ 5] = 0x01;
	tmp[ 6] = 0x05;		//ecdsa_secp384r1_sha384
	tmp[ 7] = 0x03;
	tmp[ 8] = 0x08;		//rsa_pss_rsae_sha384
	tmp[ 9] = 0x05;
	tmp[10] = 0x05;		//rsa_pkcs1_sha384
	tmp[11] = 0x01;
	tmp[12] = 0x08;		//rsa_pss_rsae_sha512
	tmp[13] = 0x06;
	tmp[14] = 0x06;		//rsa_pkcs1_sha512
	tmp[15] = 0x01;
	tmp[16] = 0x02;		//rsa_pkcs1_sha1
	tmp[17] = 0x01;
	j = 18;

	dst[4] = j >> 8;
	dst[5] = j & 0xff;
	j += 2;

	dst[0] = 0x00;
	dst[1] = 0x0d;
	dst[2] = j >> 8;
	dst[3] = j & 0xff;
	return j+4;
}
//0010 = application layer protocol negotiation
int tls1v3_clienthello_extension_0010(u8* dst)
{
	int j;
	u8* tmp = dst+6;

	//ALPN Protocol
	j = 0;
	tmp[j] = mysnprintf(tmp+j+1, 20, "h2");
	j += 1+tmp[j];
	tmp[j] = mysnprintf(tmp+j+1, 20, "http/1.1");
	j += 1+tmp[j];

	//ALPN Length
	dst[4] = j >> 8;
	dst[5] = j & 0xff;
	j += 2;

	//this type
	dst[0] = 0x00;
	dst[1] = 0x10;
	dst[2] = j >> 8;
	dst[3] = j & 0xff;
	return j+4;
}
//0012 = signed certificate timestamp
int tls1v3_clienthello_extension_0012(u8* dst)
{
	dst[0] = 0x00;
	dst[1] = 0x12;
	dst[2] = 0;
	dst[3] = 0;
	return 4;
}
//0015 = padding
int tls1v3_clienthello_extension_0015(u8* dst)
{
	int j;
	u8* tmp = dst+4;
	for(j=0;j<128;j++)tmp[j] = 0;

	dst[0] = 0x00;
	dst[1] = 0x15;
	dst[2] = j >> 8;
	dst[3] = j & 0xff;
	return j+4;
}
//0017 = extended master secret
int tls1v3_clienthello_extension_0017(u8* dst)
{
	dst[0] = 0x00;
	dst[1] = 0x17;
	dst[2] = 0;
	dst[3] = 0;
	return 4;
}
//001b = pacompress certificate
int tls1v3_clienthello_extension_001b(u8* dst)
{
	int j;
	u8* tmp = dst+4;

	tmp[0] = 2;
	tmp[1] = 0;
	tmp[2] = 2;
	j = 3;

	dst[0] = 0x00;
	dst[1] = 0x1b;
	dst[2] = j >> 8;
	dst[3] = j & 0xff;
	return j+4;
}
//0023 = session ticket
int tls1v3_clienthello_extension_0023(u8* dst)
{
	dst[0] = 0x00;
	dst[1] = 0x23;
	dst[2] = 0;
	dst[3] = 0;
	return 4;
}
//002b = supported versions
int tls1v3_clienthello_extension_002b(u8* dst)
{
	int j;
	u8* ext = dst+5;

	ext[0] = 0xca;
	ext[1] = 0xca;
	ext[2] = 0x03;
	ext[3] = 0x04;
	ext[4] = 0x03;
	ext[5] = 0x03;
	ext[6] = 0x03;
	ext[7] = 0x02;
	ext[8] = 0x03;
	ext[9] = 0x01;
	j = 10;

	dst[4] = j;
	j++;

	dst[0] = 0x00;
	dst[1] = 0x2b;
	dst[2] = j >> 8;
	dst[3] = j & 0xff;
	return j+4;
}
//002d = psk key exchange modes
int tls1v3_clienthello_extension_002d(u8* dst)
{
	dst[0] = 0x00;
	dst[1] = 0x2d;
	dst[2] = 0;
	dst[3] = 2;
	dst[4] = 1;
	dst[5] = 1;
	return 6;
}
//0033 = key share
int tls1v3_clienthello_extension_0033(u8* dst)
{
	int j,t;
	u8* tmp = dst+6;
	u8* key;

	//entry: GREASE
	tmp[0] = 0xaa;
	tmp[1] = 0xaa;
	tmp[2] = 0x00;
	tmp[3] = 0x01;
	tmp[4] = 0x00;
	j = 5;

	//entry: x25519
	key = tmp+j+4;
	for(t=0;t<0x20;t++)key[t] = getrandom()&0xff;

	tmp[j+0] = 0x00;
	tmp[j+1] = 0x1d;
	tmp[j+2] = t >> 8;
	tmp[j+3] = t & 0xff;
	j += t+4;

	//client key share length
	dst[4] = j >> 8;
	dst[5] = j & 0xff;
	j += 2;

	dst[0] = 0x00;
	dst[1] = 0x33;
	dst[2] = j >> 8;
	dst[3] = j & 0xff;
	return j+4;
}
//5a5a = GREASE
int tls1v3_clienthello_extension_5a5a(u8* dst)
{
	dst[0] = 0x5a;
	dst[1] = 0x5a;
	dst[2] = 0;
	dst[3] = 0;
	return 4;
}
//eaea = GREASE
int tls1v3_clienthello_extension_eaea(u8* dst)
{
	dst[0] = 0xea;
	dst[1] = 0xea;
	dst[2] = 0x00;
	dst[3] = 0x01;
	dst[4] = 0x00;
	return 5;
}
//ff01 = renegotiation info
int tls1v3_clienthello_extension_ff01(u8* dst)
{
	dst[0] = 0xff;
	dst[1] = 0x01;
	dst[2] = 0x00;
	dst[3] = 0x01;
	dst[4] = 0x00;
	return 5;
}




int tls1v3_clienthello(u8* dst, int len)
{
	int j,k,t;
	u8* tmp = dst+5;
	u8* ctx = tmp+4;
	u8* ext;

	//version
	ctx[0] = 0x03;
	ctx[1] = 0x03;
	k = 2;

	//random
	for(t=0;t<0x20;t++)ctx[k+t] = getrandom()&0xff;
	k += 0x20;

	//session id
	ctx[k] = 0x20;
	k += 1;
	for(t=0;t<0x20;t++)ctx[k+t] = getrandom()&0xff;
	k += 0x20;

	//cipher suite
	ctx[k+0] = 0;
	ctx[k+1] = 34;
	k += 2;
	for(t=0;t<34;t++)ctx[k+t] = clientcipher[t];
	k += 34;

	//compress method
	ctx[k+0] = 1;
	ctx[k+1] = 0;
	k += 2;

	//extensions
	ext = ctx+k+2;
	t = 0;
	t += tls1v3_clienthello_extension_5a5a(ext+t);
	t += tls1v3_clienthello_extension_0000(ext+t);
	t += tls1v3_clienthello_extension_0017(ext+t);
	t += tls1v3_clienthello_extension_ff01(ext+t);
	t += tls1v3_clienthello_extension_000a(ext+t);
	t += tls1v3_clienthello_extension_000b(ext+t);
	t += tls1v3_clienthello_extension_0023(ext+t);
	t += tls1v3_clienthello_extension_0010(ext+t);
	t += tls1v3_clienthello_extension_0005(ext+t);
	t += tls1v3_clienthello_extension_000d(ext+t);
	t += tls1v3_clienthello_extension_0012(ext+t);
	t += tls1v3_clienthello_extension_0033(ext+t);
	t += tls1v3_clienthello_extension_002d(ext+t);
	t += tls1v3_clienthello_extension_002b(ext+t);
	t += tls1v3_clienthello_extension_001b(ext+t);
	t += tls1v3_clienthello_extension_eaea(ext+t);
	t += tls1v3_clienthello_extension_0015(ext+t);
	ctx[k+0] = t >> 8;
	ctx[k+1] = t & 0xff;
	k += t+2;

	//clienthello
	tmp[0] = 0x01;
	tmp[1] = (k>>16)&0xff;
	tmp[2] = (k>> 8)&0xff;
	tmp[3] = (k>> 0)&0xff;
	j = k+4;

	//handshake
	dst[0] = 0x16;
	dst[1] = 0x03;
	dst[2] = 0x01;
	dst[3] = j >> 8;
	dst[4] = j & 0xff;
	return j+5;
}




int tls1v3client_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int tls1v3client_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int j;
	say("@tls1v3client_write:len=%x\n",len);
	printmemory(buf, len < 16 ? len : 16);

	while(1){
		if(len <= 5)break;

		j = (buf[3]<<8) + buf[4];
		if(j > len-5)break;

		switch(buf[0]){
		case 0x16:{
			say("server hello, len=%x\n",j);
			break;
		}
		case 0x14:{
			say("change cipher spec, len=%x\n",j);
			break;
		}
		case 0x17:{
			say("application data, len=%x\n",j);
			break;
		}
		default:{
			say("unknown: typ=%x,len=%x\n", buf[0], j);
			printmemory(buf, j+5);
			break;
		}
		}

		buf += 5+j;
		len -= 5+j;
	}
	return 0;
}
int tls1v3client_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tls1v3client_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@tls1v3client_linkup:%.4s\n",&self->flag);
	if(_src_ == self->flag){
		struct artery* art = self->pchip;
		u8* buf = art->buf0;
		int ret = tls1v3_clienthello(buf, 0x10000);
		give_data_into_peer(art,_src_, 0,0, 0,0, buf,ret);
	}
	return 0;
}
int tls1v3client_dartte(struct artery* art)
{
	return 0;
}
int tls1v3client_create(struct artery* art, u8* url)
{
	say("@tls1v3client_create\n");
	art->buf0 = memorycreate(0x100000, 0);
	return 0;
}




int tls1v3server_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int tls1v3server_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	say("@tls1v3server_write\n");
	return 0;
}
int tls1v3server_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tls1v3server_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tls1v3server_dartte(struct artery* art)
{
	return 0;
}
int tls1v3server_create(struct artery* art, u8* url)
{
	return 0;
}




int tls1v3master_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int tls1v3master_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	say("@tls1v3master_write\n");
	printmemory(buf,len);
	return 0;
}
int tls1v3master_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tls1v3master_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@tls1v3master_linkup\n");
	return 0;
}
int tls1v3master_dartte(struct artery* art)
{
	return 0;
}
int tls1v3master_create(struct artery* art, u8* url)
{
	say("@tls1v3master_create\n");
	return 0;
}
