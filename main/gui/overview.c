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
	for(y=320-128;y<320+128;y++)
	{
		for(x=0;x<256;x++)
		{
			point(x,y,0xff00ff);
		}
	}
	string(0x10,20,"left");
	for(y=320-128;y<320+128;y++)
	{
		for(x=512-128;x<512+128;x++)
		{
			point(x,y,0x44444444);
		}
	}
	string(0x40,20,"middle");
	for(y=320-128;y<320+128;y++)
	{
		for(x=1024-256;x<1024;x++)
		{
			point(x,y,0xffff);
		}
	}
	string(0x70,20,"right");
	for(y=0;y<128;y++)
	{
		for(x=0;x<128;x++)
		{
			point(x,y,0xffffffff);
		}
	}
	string(0,5,"overview");
	for(y=0;y<128;y++)
	{
		for(x=896;x<1024;x++)
		{
			point(x,y,0xff0000);
		}
	}
	string(0x70,5,"close");
	for(y=640-128;y<640;y++)
	{
		for(x=0;x<128;x++)
		{
			point(x,y,0xff);
		}
	}
	string(0,35,"hex");
	for(y=640-128;y<640;y++)
	{
		for(x=896;x<1024;x++)
		{
			point(x,y,0xff00);
		}
	}
	string(0x70,35,"console");
}