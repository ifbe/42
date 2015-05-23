#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void overview()
{
	int x,y;
	DWORD color;
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0xe0e0e0e0);
		}
	}
	//上下
	for(y=0;y<16;y++)
	{
		color=(QWORD)y*0x0e0e0e;

		for(x=y;x<1024-y;x++)
		{
			point(x,y,color);
			point(x,639-y,color);
		}
	}
	//左右
	for(x=0;x<16;x++)
	{
		color=(QWORD)x*0x0e0e0e;

		for(y=x;y<640-x;y++)
		{
			point(x,y,color);
			point(1023-x,y,color);
		}
	}
	//5个框
	for(y=80;y<240;y++)
	{
		for(x=0;x<256;x++)
		{
			point(x,y,0xff00ff);
		}
	}
	string(0x10,10,"left");
	for(y=80;y<240;y++)
	{
		for(x=512-128;x<512+128;x++)
		{
			point(x,y,0xffffff);
		}
	}
	string(0x40,10,"middle");
	for(y=80;y<240;y++)
	{
		for(x=768;x<1024;x++)
		{
			point(x,y,0xffff);
		}
	}
	string(0x70,10,"right");
	for(y=400;y<560;y++)
	{
		for(x=128;x<256+128;x++)
		{
			point(x,y,0xff);
		}
	}
	string(0x20,30,"hex");
	for(y=400;y<560;y++)
	{
		for(x=768-128;x<768+128;x++)
		{
			point(x,y,0xff00);
		}
	}
	string(0x60,30,"console");
}