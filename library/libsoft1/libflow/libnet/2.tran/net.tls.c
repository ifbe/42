#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void sha512sum(void* dst, void* src, int len);
void rsa2048(
        u8* dstbuf, int dstlen,
        u8* srcbuf, int srclen,
        u8* keybuf, int keylen,
        u8* modbuf, int modlen);
int pem2bin(  void* dest, void* mem, int off, int len);
int readfile( void* file, void* mem, int off, int len);
int writefile(void* file, void* mem, int off, int len);
//
void printmemory(void*, int);
void say(void*, ...);




//letsencrypt
static u8 cert_first[0x1000];
static u8 cert_second[0x1000];
static u8 cert_private[256];
static u8 cert_modulus[256];




//testonly
static u8 fixed[19]= {
	0x30,0x51,0x30,0x0d,0x06,0x09,0x60,0x86,
	0x48,0x01,0x65,0x03,0x04,0x02,0x03,0x05,
	0x00,0x04,0x40
};
static u8 clientrandom[0x20];
static u8 serverrandom[0x20] = {
	//0000
	0x4f,0xcc,0x1e,0x94,0xb7,0xe0,0x12,0xae,
	0x88,0x49,0x61,0x3b,0x0a,0xc7,0x5f,0xed,
	//0010
	0x90,0x01,0xcd,0x7d,0xc2,0xa4,0x9d,0xfa,
	0x0c,0x2e,0xe0,0x93,0x56,0xa2,0x02,0x7f
};
static u8 dh[] = {
	0x04,0x36,0xfb,0xdf,0xda,0x82,0xf5,0xc3,
	0x5e,0x4b,0x9f,0x90,0x24,0x64,0x13,0xa1,
	0x1c,0x2c,0xff,0x4c,0xbc,0xc2,0x74,0x40,
	0xc9,0x41,0xfb,0xc1,0x0a,0x14,0x63,0x23,
	0x17,0xb1,0xd8,0x15,0x62,0x25,0x26,0xc5,
	0x8f,0x05,0xba,0x34,0x19,0x52,0xb5,0x12,
	0x19,0x0b,0xb7,0x97,0x0d,0xe8,0xc7,0xc0,
	0x70,0x61,0x1f,0x49,0xb2,0x8d,0x22,0x8f,
	0x61
};




int tls_read_client_hello(u8* buf, int len)
{
	int j, k;
	u8* p = buf;

	//head
	say("type=0x%x\n", p[0]);
	say("version=%02x%02x\n", p[1], p[2]);
	len = (p[3]<<8) + p[4];
	say("length=%x\n\n", len);
	p += 5;

	//body
	say("handshake=%x\n", p[0]);
	j = (p[1]<<16) + (p[2]<<8) + p[3];
	say("length=%x\n", j);
	say("version=%02x%02x\n\n", p[4], p[5]);
	p += 6;

	//random
	say("random(len=0x20)\n");
	for(j=0;j<0x20;j++)clientrandom[j] = p[j];
	printmemory(p, 0x20);
	say("\n");
	p += 0x20;

	//sessionid
	j = p[0];
	say("sessionid(len=%x)\n", j);
	printmemory(p+1, j);
	say("\n");
	p += 1 + j;

	//cipher
	j = (p[0]<<8) + p[1];
	say("ciphersites(len=%x)\n", j);
	printmemory(p+2, j);
	say("\n");
	p += 2 + j;

	//compression
	j = p[0];
	say("compression(len=%x)\n", j);
	printmemory(p+1, j);
	say("\n");
	p += 1 + j;

	//extension
	j = (p[0]<<8) + p[1];
	say("extension(len=%x)\n", j);
	printmemory(p+2, j);
	p += 2;

	//
	while(1)
	{
		if(p-buf >= len)return 0;
		if(p-buf >= len+5)return 0;

		j = (p[0]<<8) + p[1];
		k = (p[2]<<8) + p[3];
		say("type=%04x, len=%x\n", j, k);

		p += 4 + k;
	}

	return len + 5;
}




int tls_read_server_hello(u8* buf, int len)
{
	return 0;
}
int tls_read_server_certificate(u8* buf, int len)
{
	return 0;
}
int tls_read_server_keyexch(u8* buf, int len)
{
	return 0;
}
int tls_read_server_done(u8* buf, int len)
{
	return 0;
}




int tls_read_client_keyexch(u8* buf, int len)
{
	return 0;
}
int tls_read_client_cipherspec(u8* buf, int len)
{
	return 0;
}
int tls_read_client_hellorequest(u8* buf, int len)
{
	return 0;
}




int tls_read_server_newsession(u8* buf, int len)
{
	return 0;
}
int tls_read_server_cipherspec(u8* buf, int len)
{
	return 0;
}
int tls_read_server_encrypthandshake(u8* buf, int len)
{
	return 0;
}




int tls_read_both_data(u8* buf, int len)
{
	//head
	say("type=%02x\n", buf[0]);
	say("version=%02x%02x\n", buf[1], buf[2]);
	len = (buf[3]<<8) + buf[4];
	say("length=%x\n",len);

	//data
	return len;
}
int tls_read(u64* p, u8* buf, u64 len)
{
	int ret;
	say("tls{\n");

	if(buf[0] == 0x17)
	{
		ret = tls_read_both_data(buf,len);
	}
	else if(buf[0] == 0x16)
	{
		if(buf[5] == 1)
		{
			ret = tls_read_client_hello(buf, len);
		}
		else if(buf[5] == 2)
		{
			ret = tls_read_server_hello(buf, len);
			ret += tls_read_server_certificate(buf+ret, len);
			ret += tls_read_server_keyexch(buf+ret, len);
			ret += tls_read_server_done(buf+ret, len);
		}
		else if(buf[5] == 12)
		{
			ret = tls_read_client_keyexch(buf, len);
			ret += tls_read_client_cipherspec(buf+ret, len);
			ret += tls_read_client_hellorequest(buf+ret, len);
		}
		else if(0)
		{
			ret = tls_read_server_newsession(buf, len);
			ret += tls_read_server_cipherspec(buf+ret, len);
			ret += tls_read_server_encrypthandshake(buf+ret, len);
		}
	}
	else
	{
		printmemory(buf, len);
		ret = -1;
	}

	say("}tls\n");
	return ret;
}




//1:	client >>>> server
int tls_write_client_hello(u8* buf, int len)
{
	u8* p = buf + 9;

	//version
	p[0] = p[1] = 0x3;
	p += 2;

	//random
	//*(u32*)(p+0) = getrandom();
	//*(u32*)(p+0x4) = getrandom();
	//*(u32*)(p+0x8) = getrandom();
	//*(u32*)(p+0xc) = getrandom();
	//*(u32*)(p+0x10) = getrandom();
	//*(u32*)(p+0x14) = getrandom();
	//*(u32*)(p+0x18) = getrandom();
	//*(u32*)(p+0x1c) = getrandom();
	p += 0x20;

	//sessionid length
	p[0] = 0;
	p++;

	//ciphersuites
	p[0] = 0;
	p[1] = 0x1e;
	p += 0x20;

	//compress
	p[0] = 1;
	p[1] = 0;
	p += 2;

	//extensions
	p[0] = 0;
	p[1] = 0x63;
	p += 2 + 0x63;

	//5+4byte
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = (len>>8)&0xff;
	buf[4] = len&0xff;

	buf[5] = 1;
	buf[6] = ((len-4)>>16)&0xff;
	buf[7] = ((len-4)>>8)&0xff;
	buf[8] = (len-4)&0xff;
	return len;
}




//2:	client <<<< server
int tls_write_server_hello(u8* buf, int len)
{
	int j;
	u8* p = buf + 9;

	//version
	p[0] = p[1] = 0x3;
	p += 2;

	//random
	for(j=0;j<0x20;j++)p[j] = serverrandom[j];
	p += 0x20;

	//sessionid length
	p[0] = 0;
	p++;

	//ciphersuites
	p[0] = 0xc0;
	p[1] = 0x30;
	p += 2;

	//compress
	p[0] = 0;
	p += 1;

	//extensions
	p[0] = 0;
	p[1] = 0x11;
	p += 2;

	//.ff01
	p[0] = 0xff;
	p[1] = 1;
	p[2] = 0;
	p[3] = 1;
	p[4] = 0;
	p += 5;

	//.000b
	p[0] = 0;
	p[1] = 0xb;
	p[2] = 0;
	p[3] = 4;
	p[4] = 3;
	p[5] = 0;
	p[6] = 1;
	p[7] = 2;
	p += 8;

	//.0023
	p[0] = 0;
	p[1] = 0x23;
	p[2] = 0;
	p[3] = 0;
	p += 4;

	//5+4byte
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	buf[5] = 2;
	buf[6] = ((len-9)>>16)&0xff;
	buf[7] = ((len-9)>>8)&0xff;
	buf[8] = (len-9)&0xff;
	return len;
}
int tls_write_server_certificate(u8* buf, int len)
{
	int j,k;
	u8* p = buf + 5 + 4 + 3;

	//cert 1th
	k = (cert_first[1]<<8) + cert_first[2] + 3;
	for(j=0;j<k;j++)
	{
		p[j] = cert_first[j];
	}
	p += k;

	//cert 2th
	k = (cert_second[1]<<8) + cert_second[2] + 3;
	for(j=0;j<k;j++)
	{
		p[j] = cert_second[j];
	}
	p += k;

	//5
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	//4
	buf[5] = 11;
	buf[6] = ((len-9)>>16)&0xff;
	buf[7] = ((len-9)>>8)&0xff;
	buf[8] = (len-9)&0xff;

	//3
	buf[9] = ((len-12)>>16)&0xff;
	buf[10] = ((len-12)>>8)&0xff;
	buf[11] = (len-12)&0xff;

	//
	return len;
}
int tls_write_server_keyexch(u8* buf, int len)
{
	int j;
	u8* p = buf + 9;




	//curve type,name
	p[0] = 3;
	p[1] = 0;
	p[2] = 0x17;
	p += 3;

	//pubkey(p + g + Y)
	for(j=0;j<0x41;j++)
	{
		p[1+j] = dh[j];
	}
	p[0] = 0x41;
	p += 1 + 0x41;




	//hash algorithm
	p[0] = 6;
	p[1] = 1;
	p += 2;

	p[0] = 0x01;
	p[1] = 0x00;
	p += 2;
/*
	rsa2048(
		//[173byte]padding
		//0001fffffffffffffffff...ffffffff00

		//[19byte]fixed header
		3051300d 06096086 48016503 04020305 000440

		//[64byte]result value
		sha512(
			clientrandom,
			serverrandom,
			curvetype,namedcurve,pubkeylength,pubkeydata
		)
	)//with cert's private key
*/
	//put all @ [0,0x84]
	for(j=0;j<0x20;j++)p[0x00+j] = clientrandom[j];
	for(j=0;j<0x20;j++)p[0x20+j] = serverrandom[j];
	for(j=0;j<0x45;j++)p[0x40+j] = buf[9+j];
	say("c+s+p:\n");
	printmemory(p, 0x85);

	//dst@[0x10c0,0x10ff], src@[0,0x84]
	p[0x1000] = 0;
	p[0x1001] = 1;
	for(j=0x1002;j<0x10ac;j++)p[j] = 0xff;
	p[0x10ac] = 0;
	for(j=0;j<19;j++)p[0x10ad+j] = fixed[j];
	sha512sum(p+0x10c0, p, 0x20+0x20+3+1+0x41);

	say("flag+sha512:\n");
	printmemory(p+0x1000, 256);

	//dst@[0x100,0x1ff], src@[0x1000,0x10ff]
	rsa2048(
		p,            256,
		p+0x1000,     256,
		cert_private, 256,
		cert_modulus, 256
	);
	say("rsa2048:\n");
	printmemory(p, 256);

	//
	p += 0x100;




	//5+4byte
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	buf[5] = 12;
	buf[6] = ((len-9)>>16)&0xff;
	buf[7] = ((len-9)>>8)&0xff;
	buf[8] = (len-9)&0xff;
	return len;
}
int tls_write_server_done(u8* buf, int len)
{
	//5+4byte
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = 0;
	buf[4] = 4;

	//little endian
	*(u32*)(buf+5) = 0xe;
	return 9;
}




//3:	client >>>> server
int tls_write_client_keyexch(u8* buf, int len)
{
	u8* p = buf + 9;

	//pubkey
	p[0] = 0x41;
	p += 0x42;

	//5+4byte
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	buf[5] = 12;
	buf[6] = ((len-9)>>16)&0xff;
	buf[7] = ((len-9)>>8)&0xff;
	buf[8] = (len-9)&0xff;
	return len;
}
int tls_write_client_cipherspec(u8* buf, int len)
{
	buf[0] = 0x14;
	buf[1] = buf[2] = 3;
	buf[3] = 0;
	buf[4] = 1;
	buf[5] = 1;
	return 6;
}
int tls_write_client_hellorequest(u8* buf, int len)
{
	return 0;
}




//4:	client <<<< server
int tls_write_server_newsession(u8* buf, int len)
{
	return 0;
}
int tls_write_server_cipherspec(u8* buf, int len)
{
	return 0;
}
int tls_write_server_encrypthandshake(u8* buf, int len)
{
	return 0;
}




//@:	client <--> server
int tls_write_both_data(u8* buf, int len)
{
	return 0;
}
int tls_write(u64* p, u8* buf, u64 len)
{
	int ret = 1;

	if(ret == 0)
	{
		ret = tls_write_client_hello(buf, len);
	}
	else if(ret == 1)
	{
		ret = tls_write_server_hello(buf, len);
		ret += tls_write_server_certificate(buf+ret, len);
		ret += tls_write_server_keyexch(buf+ret, len);
		ret += tls_write_server_done(buf+ret, len);
	}
	else if(ret == 2)
	{
		ret = tls_write_client_keyexch(buf, len);
		ret += tls_write_client_cipherspec(buf+ret, len);
		ret += tls_write_client_hellorequest(buf+ret, len);
	}
	else if(ret == 3)
	{
		ret = tls_write_server_newsession(buf, len);
		ret += tls_write_server_cipherspec(buf+ret, len);
		ret += tls_write_server_encrypthandshake(buf+ret, len);
	}

	return ret;
}




void tls_start()
{
	int j,fl;
	u8 buf[0x2000];




	//cert1,2,3......
	fl = readfile("fullchain.pem", buf, 0, 0x2000);
	if(fl<=0){say("err@fullchain.pem:%d\n",fl);return;}

	j = pem2bin(cert_first+3, buf, 0, fl);
	if(j<=0){say("err@cert1:%d\n",j);return;}
	cert_first[0] = 0;
	cert_first[1] = (j>>8)&0xff;
	cert_first[2] = j&0xff;
	say("cert1:\n");
	printmemory(cert_first, j+3);

	j = pem2bin(cert_second+3, buf, 1, fl);
	if(j<=0){say("err@cert2:%d\n",j);return;}
	cert_second[0] = 0;
	cert_second[1] = (j>>8)&0xff;
	cert_second[2] = j&0xff;
	say("cert2:\n");
	printmemory(cert_second, j+3);




	//private and modulus
	j = readfile("privkey.pem", buf, 0, 0x2000);
	if(j<=0){say("err@privkey.pem:%d\n",j);return;}

	j = pem2bin(buf, buf, 0, j);
	if(j<=0){say("err@pvk:%d\n",j);return;}

	for(j=0;j<0x100;j++)cert_modulus[j] = buf[0xff-j+0x26];
	say("modulus:\n");
	printmemory(cert_modulus, 0x100);

	for(j=0;j<0x100;j++)cert_private[j] = buf[0xff-j+0x130];
	say("private:\n");
	printmemory(cert_private, 0x100);
}
void tls_stop()
{
}
