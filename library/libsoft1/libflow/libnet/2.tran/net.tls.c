#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int readfile(void*, void*, int, int);
//
void printmemory(void*, int);
void say(void*, ...);




static u8 random_example[] = {
	//0000
	0x4f,0xcc,0x1e,0x94,0xb7,0xe0,0x12,0xae,
	0x88,0x49,0x61,0x3b,0x0a,0xc7,0x5f,0xed,
	//0010
	0x90,0x01,0xcd,0x7d,0xc2,0xa4,0x9d,0xfa,
	0x0c,0x2e,0xe0,0x93,0x56,0xa2,0x02,0x7f
};
static u8 pubkey[] = {
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
static u8 signature[] = {
	0xa8,0xb8,0x78,0x51,0x93,0x33,0x79,0x76,
	0x7b,0x8d,0xae,0x0a,0x44,0x1a,0xa9,0xf0,
	0x5f,0x41,0xbd,0x67,0x81,0xa5,0x54,0x6a,
	0xdb,0xed,0x55,0x27,0xa8,0x95,0x05,0x36,
	0x16,0xf8,0xdd,0xc0,0x0d,0xa8,0x40,0xa0,
	0x5b,0x84,0xdc,0xbc,0xe7,0xd4,0x0b,0x9d,
	0x3d,0x9c,0x18,0x25,0x45,0x5a,0xb2,0x14,
	0xf0,0xd0,0x0e,0x29,0x7a,0x4f,0xef,0x6f,
	0x27,0x62,0x43,0x0a,0x3b,0x9a,0x9f,0xa9,
	0x84,0x9a,0xa8,0xf4,0xa6,0x13,0x6d,0xe6,
	0x5d,0x3e,0x29,0x23,0x5f,0xb8,0x5b,0x9d,
	0x7d,0xab,0x26,0x7e,0x6c,0x4f,0x32,0x36,
	0x5a,0x49,0xb8,0x6d,0xdc,0x97,0x99,0x7b,
	0x90,0xd7,0x7b,0xe1,0x56,0x10,0x59,0x49,
	0xc8,0xf7,0xe7,0x93,0x30,0x48,0x57,0x9c,
	0xa6,0x61,0x6a,0xc2,0x30,0x50,0xd3,0x17,
	0x0a,0x22,0x7c,0x12,0x83,0x1f,0x82,0xb9,
	0xd0,0x87,0x60,0x87,0x9d,0x16,0x9b,0x62,
	0x07,0xb9,0xa0,0x23,0x8d,0xf9,0x21,0x6b,
	0xac,0x22,0x7e,0x1b,0xbb,0xf5,0xbe,0xc9,
	0x62,0xdc,0xd8,0xed,0x6e,0xfb,0x4b,0x3d,
	0x09,0x6f,0x24,0xb4,0x2d,0x0a,0x6d,0x5a,
	0x4b,0x38,0xaa,0xff,0x98,0x1f,0x07,0xfd,
	0x95,0xa8,0x16,0x6f,0xc2,0x99,0x65,0xb2,
	0x6b,0x4b,0xe5,0xb6,0xcf,0x08,0x4b,0x41,
	0x2a,0x67,0x92,0xe7,0x9e,0x3e,0x5e,0x2a,
	0x76,0x5a,0x67,0xc8,0x29,0xfe,0x06,0x15,
	0xce,0x41,0x9c,0xca,0x14,0xc1,0xed,0x61,
	0xdc,0x20,0xe2,0x1a,0x73,0x83,0xe3,0x09,
	0xdc,0xfb,0x41,0x7c,0xad,0xef,0x30,0x37,
	0xc1,0x15,0xe0,0x60,0x0f,0x78,0x71,0x8d,
	0x05,0x5f,0x90,0x5c,0x14,0x29,0x23,0xf4
};




int tls_read_client_hello(u8* buf, int len)
{
	int temp1, temp2;
	u8* p = buf;

	//head
	say("type=0x%x\n", p[0]);
	say("version=%02x%02x\n", p[1], p[2]);
	len = (p[3]<<8) + p[4];
	say("length=%x\n\n", len);
	p += 5;

	//body
	say("handshake=%x\n", p[0]);
	temp1 = (p[1]<<16) + (p[2]<<8) + p[3];
	say("length=%x\n", temp1);
	say("version=%02x%02x\n\n", p[4], p[5]);
	p += 6;

	//random
	say("random(len=0x20)\n");
	printmemory(p, 0x20);
	say("\n");
	p += 0x20;

	//sessionid
	temp1 = p[0];
	say("sessionid(len=%x)\n", temp1);
	printmemory(p+1, temp1);
	say("\n");
	p += 1 + temp1;

	//cipher
	temp1 = (p[0]<<8) + p[1];
	say("ciphersites(len=%x)\n", temp1);
	printmemory(p+2, temp1);
	say("\n");
	p += 2 + temp1;

	//compression
	temp1 = p[0];
	say("compression(len=%x)\n", temp1);
	printmemory(p+1, temp1);
	say("\n");
	p += 1 + temp1;

	//extension
	temp1 = (p[0]<<8) + p[1];
	say("extension(len=%x)\n", temp1);
	printmemory(p+2, temp1);
	p += 2;

	//
	while(1)
	{
		if(p-buf >= len)return 0;
		if(p-buf >= len+5)return 0;

		temp1 = (p[0]<<8) + p[1];
		temp2 = (p[2]<<8) + p[3];
		say("type=%04x, len=%x\n", temp1, temp2);

		p += 4 + temp2;
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
		return tls_read_both_data(buf,len);
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
	}

	say("}tls\n");
	return 0;
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
	for(j=0;j<0x20;j++)p[j] = random_example[j];
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
	p[1] = 0x1a;
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

	//.0010
	p[0] = 0;
	p[1] = 0x10;
	p[2] = 0;
	p[3] = 5;
	p[4] = 0;
	p[5] = 3;
	p[6] = 2;
	p[7] = 0x68;
	p[8] = 0x32;
	p += 9;

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
	int j;
	u8* p = buf + 5 + 4 + 3;

	//cert 1th
	j = readfile("cert.bin", p+3, 0, 0x10000);
	if(j<0)j=0;	//ignore error
	p[0] = 0;
	p[1] = (j>>8)&0xff;
	p[2] = j&0xff;
	p += 3+j;

	//cert 2th
	j = readfile("chain.bin", p+3, 0, 0x10000);
	if(j<0)j=0;	//ignore error
	p[0] = 0;
	p[1] = (j>>8)&0xff;
	p[2] = j&0xff;
	p += 3+j;

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

	//pubkey
	for(j=0;j<0x41;j++)
	{
		p[1+j] = pubkey[j];
	}
	p[0] = 0x41;
	p += 1 + 0x41;

	//hash algorithm
	p[0] = 6;
	p[1] = 1;
	p += 2;

	//signature length
	for(j=0;j<0x100;j++)
	{
		p[2+j] = signature[j];
	}
	p[0] = 0x01;
	p[1] = 0x00;
	p += 2 + 0x100;

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
