#include "libsoft.h"




//
static char what[] = "mysql_native_password";




int sql_read_serverhello(u8* buf, int len)
{
	int j,sz;
	u8* p;

	p = buf;
	sz = (*(u32*)p)&0xffffff;
	logtoall("len=%x,num=%d,protocol=%d\n", sz, p[3], p[4]);
	p += 5;

	logtoall("version=%s\n", p);
	while(*p != 0)p++;
	p++;

	logtoall("thid=%x\n", *(u32*)p);
	p += 4;

	logtoall("salt=%s\n", p);
	while(*p != 0)p++;
	p++;

	logtoall(	"srvcap=%x,sevlang=%x,srvsts=%x,extcap=%x\n",
		*(u16*)p, p[2], *(u16*)(p+3), *(u16*)(p+5)	);
	p += 7;

	logtoall("plginlen=%x\n", *p);
	p++;
	while(*p == 0)p++;

	logtoall("salt=%s\n", p);
	while(*p != 0)p++;
	p++;

	logtoall("%s\n", p);
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
int sql_client(_obj* ele, int fd, u8* buf, int len)
{
	int ret;
	printmemory(buf, len);

	if(ele[fd].vfmt == 0)
	{
		ret = sql_read_serverhello(buf, len);
		if(ret <= 0)return 0;

		ret = sql_write_loginrequest(buf, len);
		if(ret <= 0)return 0;

		//writesocket(fd, 0, buf, ret);
		ele[fd].vfmt = 1;
	}
	else
	{
		logtoall("error@serve_sql\n");
	}
	return sql;
}
int sql_server(_obj* ele, int fd, u8* buf, int len)
{
	return 0;
}
