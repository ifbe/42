#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

//static char showclose=0;
//static char showconsole=0;
//static char showreal=0;
//static char showlogic=0;
static char what=0;
void killmehelpit(int killwho,int helpwho)
{
	what=helpwho;
}




void printworld()
{
	int x,y;
	//主界面显示什么
	if(what==0)
	{
		printdisk();
	}
	if(what==1)
	{
		real0();
	}
	else if(what==2)
	{
		logic0();
	}
	else if(what==3)
	{
		hex();
	}
	else if(what==4)
	{
		console();
	}
	else if(what==5)
	{
		overview();
	}

	//右上角
	for(y=0;y<16;y++)
	{
		point(1023,y,0xff0000);
		point(1023-16,y,0xff0000);
	}
	for(x=1024-16;x<1024;x++)
	{
		point(x,0,0xff0000);
		point(x,15,0xff0000);
	}
	//左上角
	for(y=640-16;y<640;y++)
	{
		for(x=0;x<16;x++)
			point(x,y,0xff);
		for(x=16;x<32;x++)
			point(x,y,0xff00);
		for(x=32;x<48;x++)
			point(x,y,0xff0000);
		for(x=48;x<64;x++)
			point(x,y,0xff00ff);
		for(x=64;x<80;x++)
			point(x,y,0xffff);
		for(x=80;x<96;x++)
			point(x,y,0xffff00);
	}
}
void main()
{
	initmaster();

	diskinit();			//0
	real0init();		//1
	logic0init();		//2
	hexinit();			//3
	consoleinit();		//4


	while(1)
	{
		//1.这次显示啥
		printworld();			//内容
		writescreen();		//写屏

		//2.等事件
		DWORD type=0;
		DWORD key=0;
		waitevent(&type,&key);
		say("type=%x,key=%x\n",type,key);
		if(type==0)return;

		//3.干啥事
		int x=key&0xffff;			//四个角落
		int y=(key>>16)&0xffff;
		if(type==1)
		{
			if(key==0x1b)what=0;		//不管谁在干活，按下esc就显示主屏
			continue;
		}
		if(type==2)
		{
			if( (y<16) && (x>1024-16) )return;				//右上
			if(y>640-16)
			{
				if(x<16)
				{
					what=0;
					continue;
				}
				else if(x<32)
				{
					if(what==1)what=0;
					else what=1;
					continue;
				}
				else if(x<48) //showconsole^=1;		//左上
				{
					if(what==2)what=0;
					else what=2;
					continue;
				}
				else if(x<64)
				{
					if(what==3)what=0;
					else what=3;
					continue;
				}
				else if(x<80)
				{
					if(what==4)what=0;
					else what=4;
					continue;
				}
				else if(x<96)
				{
					if(what==5)what=0;
					else what=5;
					continue;
				}
			}
		}

		//disk在干活就交给disk
		if(what==0)diskmessage(type,key);

		//real0在干活就交给real0
		else if(what==1)real0message(type,key);

		//logic0在干活就交给logic0
		else if(what==2)logic0message(type,key);

		//hex在干活就交给hex
		else if(what==3)hexmessage(type,key);

		//console在干活就交给console
		else if(what==4)consolemessage(type,key);

	}//while(1)
}
