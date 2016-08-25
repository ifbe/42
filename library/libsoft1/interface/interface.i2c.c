#define u8 unsigned char
#define u64 unsigned long long
//
int systemi2c_list(char*);
int systemi2c_choose(int,char*);
int systemi2c_read(u8 dev,u8 reg,u8* buf,u8 count);
int systemi2c_write(u8 dev,u8 reg,u8* buf,u8 count);
int systemi2c_start(char* p);
int systemi2c_stop();
//
int decstring2data(char*,u64*);
int hexstring2data(char*,u64*);
void say(char*,...);




static int i2c_read()
{
}
static int i2c_write()
{
}
static int i2c_list(char* p)
{
	systemi2c_list(0);
}
static int i2c_choose(char* p)
{
	int ret;
	u64 data;

	//
	if(p==0)
	{
		systemi2c_choose(-1,0);
		return 0;
	}

	//".."
	if( (p[0]=='.') && (p[1]=='.') )
	{
		systemi2c_choose(-1,p);
		return 1;
	}

	//"/dev/i2c-1"
	if(p[0]=='/')
	{
		ret=decstring2data(p+9,&data);
		if(ret<=0)return -2;

		return systemi2c_choose((int)data,p);
	}

	//"0x68"
	if(p[0]=='0'&&p[1]=='x')
	{
		ret=hexstring2data(p+2,&data);
		if(ret<=0)return -2;

		return systemi2c_choose((int)data,0);
	}

	//33
	else
	{
		ret=decstring2data(p,&data);
		if(ret<=0)return -3;

		return systemi2c_choose((int)data,0);
	}
}
static int i2c_start(u64 type,char* p)
{
	systemi2c_start(p);
}
static int i2c_stop()
{
	systemi2c_stop();
}
void i2c_create(char* world,u64* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x633269;		//id
	p[2]=(u64)i2c_start;
	p[3]=(u64)i2c_stop;
	p[4]=(u64)i2c_list;
	p[5]=(u64)i2c_choose;
	p[6]=(u64)i2c_read;
	p[7]=(u64)i2c_write;
}
void i2c_delete()
{
	i2c_stop();
}
