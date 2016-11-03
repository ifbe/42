#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void windowcreate();
void windowdelete();
void windowstart(void* addr,void* pixfmt, int width,int height);
void windowstop();
void windowread();
void windowwrite();
//
void eventwrite();
void eventread(u64* first,u64* second);




static u32 palette[1024*1024];
static u32 color;



//lord's requires
void printworld()
{
	int x,y;
	for(y=0;y<1024;y++)
	{
		for(x=0;x<1024;x++)
		{
			palette[y*1024+x]=color;
		}
	}
}
void processmessage(u64 type,u64 key)
{
	color=key;
}
void main()
{
	//
	windowcreate();
	windowstart(palette, "rgba8888", 1024, 1024);

	//forever
	u64 type=0;
	u64 key=0;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		printworld();
		windowwrite();

		//2.等事件，是退出消息就退出
		eventread(&type,&key);
		if( type==0 )break;
		if( (type==0x64626b)&&(key==0x1b))break;


		//3.处理事件，如果要求自杀就让它死
		processmessage(type,key);
	}

	//after
	windowstop();
	windowdelete();
}
