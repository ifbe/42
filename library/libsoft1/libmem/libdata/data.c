#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void md5sum(void*, void*, int);
void sha1sum(void*, void*, int);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int startfile(u8*);
int stopfile(u64);
int readfile(u64, u8*, u64, u64);
int writefile(u64, u8*, u64, u64);
//
void printmemory(void*, int);
void say(void*, ...);




//
static u8* fdhome;
static u8* fshome;
static u8* dirhome;
static u8* datahome;
static int algorithm = 0;
void explain_data(u8* buf, int len)
{
	char res[0x40];
	md5sum(res, buf, len);
	printmemory(res, 16);
}




static int data_ls()
{
	return 0;
}
static int data_cd(u8* p)
{
	int fd;
	int ret;
/*
	//open
	fd = startfile(p);
	if(fd <= 0)return -1;

	//read
	ret = readfile(fd, datahome, 0, 0x100000);
	if(ret < 0)return 0;

	//.......
	explain_data(datahome, ret);

	//close
	stopfile(fd);
*/

	//..
	if(cmp(p, "..") == 0)
	{
		algorithm = 0;
	}

	//crypt
	else if(cmp(p, "dsa") == 0)
	{
		algorithm = 6;
	}
	else if(cmp(p, "rsa") == 0)
	{
		algorithm = 7;
	}
	else if(cmp(p, "des") == 0)
	{
		algorithm = 8;
	}
	else if(cmp(p, "aes") == 0)
	{
		algorithm = 9;
	}

	//encode
	else if(cmp(p, "base64") == 0)
	{
		algorithm = 1;
	}
	else if(cmp(p, "pem") == 0)
	{
		algorithm = 1;
	}
	else if(cmp(p, "x509") == 0)
	{
		algorithm = 1;
	}
	else if(cmp(p, "barcode") == 0)
	{
		algorithm = 1;
	}
	else if(cmp(p, "qrcode") == 0)
	{
		algorithm = 1;
	}

	//hash
	else if(cmp(p, "md5") == 0)
	{
		algorithm = 1;
	}
	else if(cmp(p, "sha1") == 0)
	{
		algorithm = 2;
	}
	else if(cmp(p, "sha256") == 0)
	{
		algorithm = 3;
	}
	else if(cmp(p, "sha384") == 0)
	{
		algorithm = 4;
	}
	else if(cmp(p, "sha512") == 0)
	{
		algorithm = 5;
	}

	//search
	else if(cmp(p, "bm") == 0)
	{
		algorithm = 14;
	}
	else if(cmp(p, "kmp") == 0)
	{
		algorithm = 15;
	}

	//sort
	else if(cmp(p, "bubble") == 0)
	{
		algorithm = 10;
	}
	else if(cmp(p, "gnome") == 0)
	{
		algorithm = 11;
	}
	else if(cmp(p, "heap") == 0)
	{
		algorithm = 12;
	}
	else if(cmp(p, "merge") == 0)
	{
		algorithm = 13;
	}
	else if(cmp(p, "quick") == 0)
	{
		algorithm = 13;
	}
	else if(cmp(p, "select") == 0)
	{
		algorithm = 13;
	}

	//
	say("%d\n", algorithm);
	return 0;
}
static int data_read(u8* addr)
{
	return 0;
}
static int data_write()
{
	return 0;
}
static int data_start()
{
	return 0;
}
static int data_stop()
{
	return 0;
}
int data_create(void* softaddr, u64* p)
{
	fdhome = softaddr;
	fshome = softaddr + 0x100000;
	dirhome = softaddr + 0x200000;
	datahome = softaddr + 0x300000;

	//
	p[0]=0x79726f6d656d;
	p[1]=0x61746164;
	p[2]=(u64)data_start;
	p[3]=(u64)data_stop;
	p[4]=(u64)data_ls;
	p[5]=(u64)data_cd;
	p[6]=(u64)data_read;
	p[7]=(u64)data_write;

	return 0x100;
}
int data_delete()
{
	return 0;
}
