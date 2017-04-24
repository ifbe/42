#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);
//
void printmemory(void*, int);
void say(void*, ...);




struct object
{
	//[0x00,0x0f]
	u64 type_sock;  //raw, bt, udp, tcp?
	u64 stage0;
	u64 type_road;  //ssh, tls?
	u64 stage1;
	u64 type_app;   //http2, ws, rdp, vnc?
	u64 stage2;
	u64 type_data;  //html, rgb?
	u64 stage3;

	//[0x40,0x7f]
	u8 self[0x20];
	u8 peer[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};
static char what[] = "mysql_native_password";




int sql_read_serverhello(u8* buf, int len)
{
	int j,sz;
	u8* p;

	p = buf;
	sz = (*(u32*)p)&0xffffff;
	say("len=%x,num=%d,protocol=%d\n", sz, p[3], p[4]);
	p += 5;

	say("version=%s\n", p);
	while(*p != 0)p++;
	p++;

	say("thid=%x\n", *(u32*)p);
	p += 4;

	say("salt=%s\n", p);
	while(*p != 0)p++;
	p++;

	say(	"srvcap=%x,sevlang=%x,srvsts=%x,extcap=%x\n",
		*(u16*)p, p[2], *(u16*)(p+3), *(u16*)(p+5)	);
	p += 7;

	say("plginlen=%x\n", *p);
	p++;
	while(*p == 0)p++;

	say("salt=%s\n", p);
	while(*p != 0)p++;
	p++;

	say("%s\n", p);
	return len;
}
int sql_write_loginrequest(u8* buf, int len)
{
	int j;
	u8* p = buf+4;

	*(u16*)p = 0xa685;
	*(u16*)(p+2) = 0x203f;
	*(u32*)(p+4) = 0x1000000;
	p[8] = 0x2d;
	p += 9;

	//padding
	for(j=0;j<23;j++)
	{
		*p = 0;
		p++;
	}

	//username
	*(u32*)p = 0x746f6f72;
	p[4] = 0;
	p += 5;

	//password
	*p = 0x14;
	for(j=0;j<20;j++)p[j+1] = 0x30+j;
	p += 0x14+1;

	//string
	for(j=0;j<sizeof(what);j++)p[j] = what[j];
	p += j;

	p[0] = 0x72;
	for(j=0;j<0x72;j++)p[j+1] = 0;
	p += 0x73;

	len = p-buf;
	*(u32*)buf = len-4;
	buf[3] = 1;

	return len;
}




#define SQL 0x4c5153
#define sql 0x6c7173
int serve_sql(struct object* obj, int fd, u8* buf, int len)
{
	int ret;
	printmemory(buf, len);

	if(obj[fd].stage1 == 0)
	{
		ret = sql_read_serverhello(buf, len);
		if(ret <= 0)return 0;

		ret = sql_write_loginrequest(buf, len);
		if(ret <= 0)return 0;

		writesocket(fd, buf, 0, ret);
		obj[fd].stage1 = 1;
	}
	else
	{
		say("error@serve_sql\n");
	}
	return sql;
}
