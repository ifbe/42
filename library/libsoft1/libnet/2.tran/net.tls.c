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




int tls_write_data(u8* buf, int len)
{
	u8* p = buf;

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
	//p += 0x20;

	//
	p[0] = 0;

	//
	return len;
}
int tls_write_body(u8* buf, int len)
{
	//type
	buf[0] = 2;

	//len
	buf[1] = (len>>16)&0xff;
	buf[2] = (len>>8)&0xff;
	buf[3] = len&0xff;

	//
	return len+4;
}
int tls_write_head(u8* buf, int len)
{
	buf[0] = 0x22;
	buf[1] = buf[2] = 0x3;
	buf[3] = (len>>8)&0xff;
	buf[4] = len&0xff;
}
int tls_write(u64* p, u8* buf, u64 len)
{
	int ret;

	//data
	ret = tls_write_body(buf+9, ret);

	//body
	ret = tls_write_body(buf+5, ret);

	//head
	ret = tls_write_head(buf, ret);

	return 1;
}
