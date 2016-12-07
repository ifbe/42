#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*, int);
void say(char*, ...);




int tls_read_head(u8* buf, int len)
{
        int length;

        //0
        say("type=0x%x\n",buf[0]);

        //1,2
        say("version=%02x%02x\n",buf[1],buf[2]);

        //3,4
        length = (buf[3]<<8) + buf[4];
        say("length=%x\n",length);

	//
        return length;
}
int tls_read_body(u8* buf, int len)
{
	int totallength, cipherlength, extralength;
        int offset, temp1, temp2;

        //0
        say("handshake=%x\n", buf[0]);

        //1,2,3
        totallength = (buf[1]<<16) + (buf[2]<<8) + buf[3];
        say("total length=%x\n",totallength);

        //4,5
        say("version=%02x%02x\n",buf[4],buf[5]);

        //[6,0x25]
        printmemory(buf+6, 0x20);

        //0x26
        say("sessionid length=%x\n",buf[0x26]);

        //0x27,0x28
        cipherlength = (buf[0x27]<<8) + buf[0x28];
        say("ciphersites length=%x\n",cipherlength);

        //
        printmemory(buf+0x29, cipherlength);
        offset = 0x29+cipherlength;

        //
        say("compression length=%x\n",buf[offset]);
        say("compression method=%x\n",buf[offset+1]);
        offset += 2;

        //
        extralength = (buf[offset]<<8) + buf[offset+1];
        say("extension length=%x\n", extralength);
        offset += 2;

        //
        printmemory(buf+offset, extralength);
        while(1)
        {
                if(offset >= totallength)break;

                temp1 = (buf[offset+0]<<8) + buf[offset+1];
                temp2 = (buf[offset+2]<<8) + buf[offset+3];
                say("type=%04x, len=%x\n", temp1, temp2);

                offset += 4 + temp2;
        }

	return totallength;
}
int tls_read(u64* p, u8* buf, u64 len)
{
	int ret;
	say("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@{\n");

	//
	ret = tls_read_head(buf, len);

	//
	ret = tls_read_body(buf+5, len-5);

	say("}@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	return 1;
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
}
int tls_write_server_cipherspec(u8* buf, int len)
{
}
int tls_write_server_encrypthandshake(u8* buf, int len)
{
}




//@:	client <--> server
int tls_write_both_data(u8* buf, int len)
{
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
