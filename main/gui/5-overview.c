#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void overview()
{
	int x,y;
	background0();

	//外面的口
	for(y=128;y<640-128;y++)point(256,y,0);
	for(y=128;y<640-128;y++)point(767,y,0);
	for(x=256;x<768;x++)point(x,128,0);
	for(x=256;x<768;x++)point(x,639-128,0);

	//撇捺
	for(y=0;y<128;y++)
	{
		point(512-64-y,y+128+64,0);
		point(512+64+y,y+128+64,0);
	}

	//里面的口
	for(y=320;y<320+128;y++)point(256+128,y,0);
	for(y=320;y<320+128;y++)point(767-128,y,0);
	for(x=256+128;x<768-128;x++)point(x,320,0);
	for(x=256+128;x<768-128;x++)point(x,320+128,0);
}
/*
void overview()
{
	int x,y;
	background0();
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
}*/