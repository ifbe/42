#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void overview()
{
	int x,y;
	//background0();

	//外面的口
	for(y=128;y<640-128;y++)
	{
		for(x=256;x<768;x++)
		{
			point(x,y,0xffff00);
		}
	}
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
	for(y=320;y<320+128;y++)
	{
		for(x=256+128;x<768-128;x++)
		{
			point(x,y,0xffff);
		}
	}
	for(y=320;y<320+128;y++)point(256+128,y,0);
	for(y=320;y<320+128;y++)point(767-128,y,0);
	for(x=256+128;x<768-128;x++)point(x,320,0);
	for(x=256+128;x<768-128;x++)point(x,320+128,0);

	//
	string(0x38,16,"every where to live");
	string(0x38,24,"only here to die");
}
void overviewmessage(DWORD type,DWORD key)
{
	if(type==2)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		if(x>256+128)
		{
			if(x<768-128)
			{
				if(y>320)
				{
					if(y<320+128)
					{
						chooseoperator(0xff);		//请求退出
						return;
					}
				}
			}
		}
		chooseoperator(0x80);		//请求前一个
	}
}