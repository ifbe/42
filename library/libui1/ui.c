#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void xt100_read(void*);
void xt100_write(void*);
//
int ncmp(void*,void*,int);
int cmp(void*,void*);
u32 getrandom();
//
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




struct window
{
	u64 buf1;
	u64 buf2;
	u64 fmt;
	u64 dim;

	u64 w;
	u64 h;
	u64 d;
	u64 t;

	char data[0xc0];
};
struct working
{
	//[0,7]:种类
	u64 type;

	//[8,f]:名字
	u64 name;

	//[10,17]:开始
	int (*start)();
	char padding2[ 8 - sizeof(char*) ];

	//[18,1f]:结束
	int (*stop)();
	char padding3[ 8 - sizeof(char*) ];

	//[20,27]:观察
	int (*list)();
	char padding4[ 8 - sizeof(char*) ];

	//[28,2f]:调整
	int (*choose)();
	char padding5[ 8 - sizeof(char*) ];

	//[30,37]:输出
	int (*read)(void* win);
	char padding6[ 8 - sizeof(char*) ];

	//[38,3f]:输入
	int (*write)(void* event);
	char padding7[ 8 - sizeof(char*) ];

	char data[0xc0];
};
static struct window* win = 0;
static struct working* worker = 0;
//
static u32 now=0;		//不能有负数
static u32 menu=0;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void charactercreate(u8* type, u8* addr)
{
	//register new workers and configure inner data
	int i;
	u8* temp;
	if(type!=0)return;
	if( (type == 0)&&(worker != 0) )return;

	//where
	win = (struct window*)(addr+0);
	worker = (struct working*)(addr+0x100000);

	//clean
	temp = (void*)worker;
	for(i=0;i<0x100000;i++)temp[i]=0;

	//library
	dimension1_create(addr, temp);
	dimension2_create(addr, temp);
	dimension3_create(addr, temp);

	//special
	external_create(addr, temp);

	//ordinary
	content_create(addr, temp);

	//
	for(now=0;now<100;now++)
	{
		if(worker[now].type != 0)break;
	}
	//say("[c,f):createed character\n");
}
void characterdelete()
{
	//say("[c,f):deleteing character\n");

	content_delete();
	external_delete();

	dimension1_delete();
	dimension2_delete();
	dimension3_delete();

	worker = 0;
	win = 0;
}
int characterstart(int j)
{
	//dumplicate existing workers and configure inner data
	worker[now].start();
	return 0;
}
int characterstop()
{
	worker[now].stop();
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int characterlist(u8* p)
{
	//列出所有“人物”
	int j;
	int ret;
	if(p == 0)
	{
		for(j=0;j<256;j++)
		{
			if(worker[j].name != 0)
			{
				say("%d:%s\n",j,&worker[j].name);
			}
		}
		return 0;
	}

	else
	{
		//start searching
		for(j=0;j<0x100;j++)
		{
			//all searched
			if(worker[j].name == 0)return 0;

			//lookat this
			//say("[%s][%s]\n",&worker[j].name, p);
			ret = ncmp(&worker[j].name, p, 8);
			if(ret == 0)return j;
		}
		return 0;
	}
}
int characterchoose(u8* p)
{
	return 0;
}
void characterread()
{
	int j;
	for(j=0;j<16;j++)
	{
		if(win[j].fmt == 0)break;
		if(win[j].fmt == 0x6563696f76)continue;
		if(win[j].fmt == 0x696c63)
		{
			xt100_read(&win[j]);
			continue;
		}
		if(win[j].fmt == 0x6c6d7468)
		{
			worker[now].read(&win[j]);
			continue;
		}

		if( (j >= 2) && (win[j].dim == 1) )
		{
			xt100_read(&win[j]);
		}
		else
		{
			worker[now].read(&win[j]);
			if(menu > 0)
			{
				worker[1].read(&win[j]);
				worker[0].read(&win[j]);
			}
		}
	}
}
void characterwrite(u64* p)
{
	int x;

	//size
	if(p[1] == 0x657a6973)
	{
		win[2].w = p[0] & 0xffff;
		win[2].h = (p[0] >> 16) & 0xffff;
		return;
	}//size

	//kbd
	else if(p[1] == 0x64626b)
	{
		//按下esc
		if(p[0] == 0x1b){menu ^= 1;return;}
		else if(p[0] == 0xf1){win[2].dim = 1;return;}
		else if(p[0] == 0xf2){win[2].dim = 2;return;}
		else if(p[0] == 0xf3){win[2].dim = 3;return;}
	}//kbd

	//
	if((win[2].fmt == 0x696c63)|(win[2].dim == 1))
	{
		xt100_write(p);
		return;
	}

	//其余所有消息，谁在干活就交给谁
	if(menu > 0)
	{
		worker[1].write(p);
		worker[0].write(p);
	}
	worker[now].write(p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int charactercommand(u8* p)
{
	int j,k,ret;
	u64 temp;

	//exit!
	if( (p==0) | (cmp(p,"exit")==0) )
	{
		say("chatacter(%d) wants to die\n",now);
		eventwrite(0,0,0,0);
		return 0;
	}

	//next
	ret=cmp(p,"+");
	if(ret==0)
	{
		if(worker[now+1].name == 0)return 0;
		now++;
		goto found;
	}

	//last
	ret=cmp(p,"-");
	if(ret==0)
	{
		if(worker[now-1].type == 0)return 0;
		now--;
		goto found;
	}

	//random
	ret=cmp(p,"random");
	if(ret==0)
	{
		for(j=0;j<10;j++)
		{
			if(worker[j].type != 0)break;	//skip menu|draw
		}
		k=j;

		for(;k<0x100000/0x80;k++)
		{
			if(worker[k].name == 0)break;
		}

		now=( getrandom() % (k-j) ) + j;
		goto found;
	}

	//search
	ret = characterlist(p);
	if(ret != 0)
	{
		now = ret;
		goto found;
	}

notfound:
	return 0;

found:
	//
	characterstart(0);
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
