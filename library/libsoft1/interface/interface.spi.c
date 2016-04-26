#define QWORD unsigned long long
#define BYTE unsigned char
//
int systemspi_list(char*);
int systemspi_choose(int,char*);
int systemspi_read(BYTE dev,BYTE reg,BYTE* buf,BYTE count);
int systemspi_write(BYTE dev,BYTE reg,BYTE* buf,BYTE count);
int systemspi_start(char* p);
int systemspi_stop();
//
int decstring2data(char*,QWORD*);
int hexstring2data(char*,QWORD*);
void say(char*,...);




static int spi_read()
{
}
static int spi_write()
{
}
static int spi_list()
{
	systemspi_list(0);
}
static int spi_choose(char* p)
{
	int ret;
	QWORD data;

	//
	if(p==0)
	{
		systemspi_choose(0,0);
		return 0;
	}

	//".."
	if( (p[0]=='.') && (p[1]=='.') )
	{
		systemspi_choose(-1,p);
		return 1;
	}

	//"/dev/spidev0.0"
	if(p[0]=='/')
	{
		ret=decstring2data(p+11,&data);
		if(ret<=0)return -2;

		return systemspi_choose((int)data,p);
	}

	//"0x68"
	if(p[0]=='0'&&p[1]=='x')
	{
		ret=hexstring2data(p+2,&data);
		if(ret<=0)return -2;

		return systemspi_choose((int)data,0);
	}

	//33
	else
	{
		ret=decstring2data(p,&data);
		if(ret<=0)return -3;

		return systemspi_choose((int)data,0);
	}
}
static int spi_start(QWORD type,char* p)
{
	systemspi_start(p);
}
static int spi_stop()
{
	systemspi_stop();
}
void spi_init(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x697073;		//id
	p[2]=(QWORD)spi_start;
	p[3]=(QWORD)spi_stop;
	p[4]=(QWORD)spi_list;
	p[5]=(QWORD)spi_choose;
	p[6]=(QWORD)spi_read;
	p[7]=(QWORD)spi_write;
}
void spi_kill()
{
}
