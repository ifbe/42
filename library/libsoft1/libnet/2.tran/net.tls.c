#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void say(void*, ...);




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
	u8* p = buf + 9;

	//version
	p[0] = p[1] = 0x3;
	p += 2;

	//random
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
	u8* p = buf + 9;

	//certificate
	p[0] = 0;
	p[1] = 0x2;
	p[2] = 0x8;
	p += 3;

	//
	p[0] = 0;
	p[1] = 0x2;
	p[2] = 0x5;
	p += 3;

	//
	p += 517;

	//5+4byte
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	buf[5] = 11;
	buf[6] = ((len-9)>>16)&0xff;
	buf[7] = ((len-9)>>8)&0xff;
	buf[8] = (len-9)&0xff;
	return len;
}
int tls_write_server_keyexch(u8* buf, int len)
{
	u8* p = buf + 9;

	//curve type,name
	p[0] = 3;
	p[1] = 0;
	p[2] = 0x17;
	p += 3;

	//pubkey
	p[0] = 0x41;
	p += 0x42;

	//hash algorithm
	p[0] = 6;
	p[1] = 1;
	p += 2;

	//signature length
	p[0] = 0;
	p[1] = 0x80;
	p += 0x82;

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
