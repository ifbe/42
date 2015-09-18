#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

void die();
//void f1init();

void string(int x,int y,char* str);
void point(int x,int y,DWORD color);
void background4(QWORD,QWORD);

int compare(char* first,char* second);
void command(char* in);

QWORD whereisworld();




//log位置
static QWORD logbuf;
static QWORD offset=0;

//键盘输入
static BYTE buffer[128];//键盘输入专用
static int bufcount=0;








void f4show()
{
	//显示哪儿开始的一块
	int x,y;
	DWORD temp=*(DWORD*)(logbuf+0xffff0);
	QWORD showaddr=temp-offset;				//代表末尾位置而不是开头
	if(showaddr<0x80*39)showaddr=0x80*39;

	//背景
	background4(showaddr,temp);

	//总共38行，必须保证showaddr>=0x80*39
	for(y=0;y<39;y++)
	{
		string(0,y,(char*)logbuf+showaddr+0x80*(y-39));
	}
	string(0,39,"user:");
	string(0x5,39,buffer);
}
void f4message(QWORD type,QWORD key)
{
	if(type==0x72616863)		//键盘
	{
		if(key==0x8)			//backspace
		{
			if(bufcount!=0)
			{
				bufcount--;
				buffer[bufcount]=0;
			}
		}
		else if(key==0xd)		//回车
		{
			if(compare( buffer , "exit" ) == 0)
			{
				die();
				return;
			}
			else
			{
				//if( ( (*(QWORD*)buffer)&0xffffffffffff ) == 0x207375636f66 )	//"focus "
				//{
				//	f1init();
				//}
				command(buffer);
			}

			int i;
			bufcount=0;
			for(i=0;i<128;i++)
			{
				buffer[i]=0;
			}
		}
		else
		{
			if(bufcount<0x80)
			{
				buffer[bufcount]=key&0xff;
				bufcount++;
			}
		}
	}
	else if(type==0x6c65656877)
	{
		if(key<0xff0000)		//滚轮上
		{
			DWORD temp=*(DWORD*)(logbuf+0xffff0);
			if(temp>=0x80*40)		//不够一页不用上翻
			{
				if(offset<temp-0x80*40)offset+=0x80;
			}
		}
		else if(key>0xff0000)	//滚轮下
		{
			if(offset>=0x80)offset-=0x80;
		}
	}
}








void f4init(QWORD world)
{
	logbuf=world+0x600000;
}