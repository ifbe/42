#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void writewindow(QWORD size,void* addr);
void waitevent(QWORD* type,QWORD* key);

void initpalette();
void cleanall();




static DWORD palette[1024*1024];
static DWORD color;



//lord's requires
void printworld()
{
	int x,y;
	for(y=0;y<512;y++)
	{
		for(x=0;x<1024;x++)
		{
			palette[y*1024+x]=color;
		}
	}
}
void processmessage(QWORD type,QWORD key)
{
	color=key;
}
void main()
{
	//before
	initwindowworker();

	//forever
	QWORD type=0;
	QWORD key=0;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		printworld();
		writewindow(0x04000400,palette);

		//2.等事件，是退出消息就退出
		waitevent(&type,&key);
		if( type==0 )break;

		//3.处理事件，如果要求自杀就让它死
		processmessage(type,key);
	}

	//after
	killwindowworker();
}
