#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int cli_read(void*);
int cli_write(void*);
void draw_vt100(void*, int, int, int, int);
void carve_term3d(void*);
//
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
//
void external_create(void*, void*);
void external_delete();
void content_create(void*, void*);
void content_delete();
//
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
static u32 newline=0;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void charactercreate(u8* type, u8* addr)
{
	int j;
	if(type != 0)return;
	if( (type == 0)&&(win != 0) )return;

	//clean
	for(j=0x100000;j<0x200000;j++)addr[j] = 0;

	//where
	win = (void*)(addr+0);
	worker = (void*)(addr+0x100000);

	//lib1d
	lib1d_create(addr, 0);

	//lib2d
	lib2d_create(addr, 0);

	//lib3d
	lib3d_create(addr, 0);

	//special
	external_create(addr, 0);

	//ordinary
	content_create(addr, 0);

	//
	for(now=0;now<1000;now++)
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

	lib1d_delete();
	lib2d_delete();
	lib3d_delete();

	worker = 0;
	win = 0;
}
int characterstart(int j)
{
	return worker[now].start();
}
int characterstop()
{
	return worker[now].stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int characterlist(u8* p)
{
	//列出所有“人物”
	int j;
	int ret;
	u64 type;
	u64 name;
	if(p == 0)
	{
		ret = 0;
		type = 0;
		name = 0;
		for(j=0;j<0x100;j++)
		{
			if(worker[j].name == 0)
			{
				if((ret%8)!=0)say("\n");
				break;
			}

			if(worker[j].type != type)
			{
				say("\n%s:\n",&worker[j].type);
				ret=0;
			}
			
			type = worker[j].type;
			name = worker[j].name;
			if((ret>0)&&(ret%8==0))say("\n");

			say("	%s", &name);
			ret++;
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
	int j,k,ret;
	u64 temp;

	//exit!
	if( (p==0) | (cmp(p,"exit")==0) )
	{
		say("%d wants to die\n",now);
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
	return now;
}
int characterread()
{
	int j;
	struct window* w;
	for(j=0;j<16;j++)
	{
		//
		w = &win[j];

		//error
		if(w->fmt == 0)break;

		//title
		//if(w->fmt == 0x)title_read(w);

		//tray
		//if(w->fmt == 0x)tray_read(w);

		//voice
		if(w->fmt == 0x6563696f76)continue;

		//1d:	cli
		if(w->dim == 1)
		{
			if(newline == 1)
			{
				worker[now].read(w);
				cli_read(w);
				newline = 0;
			}
			if(w->fmt == 0x696c63)return 0;

			draw_vt100(w, 0, 0, 0xffff, 0xffff);
			return 0;
		}

		//2d:	rgba
		else if(w->dim == 2)
		{
			worker[now].read(w);

			if(menu > 0)
			{
				worker[1].read(w);
				worker[0].read(w);
			}
			return 0;
		}

		//3d:	directx, opengl, vulkan
		else if(w->dim == 3)
		{
/*
			if(w->dim == 1)
			{
				carve_term3d(w);
			}
			else if(w->dim == 2)
			{
				carve_texture(w);
			}
			else worker[now].read(w);

			if(menu > 0)
			{
				worker[1].read(w);
				worker[0].read(w);
			}
*/
			return 0;
		}//dim=3
	}//for
	return 0;
}
int characterwrite(u64* ev)
{
	//prepare
	int ret;
	if(ev[1] == 0x727473)
	{
		ret = characterchoose( (void*)(ev[0]) );
		if(ret == 0)say("%s\n", ev[0]);
		return;
	}
	ev[2] = (u64)(&win[2]);




	//process
	u64 why = ev[0];
	u64 what = ev[1];
	struct window* w = (void*)ev[2];
	//u64 when = ev[3];

	//kbd
	if(what == 0x64626b)
	{
		if(why == 0xf1){w->dim = 1;return 0;}
		else if(why == 0xf2){w->dim = 2;return 0;}
		else if(why == 0xf3){w->dim = 3;return 0;}
	}//kbd

	//
	if(w->dim == 1)
	{
		if(what == 0x64626b)
		{
			ev[1] = 0x72616863;
			if(why == 0x1b)ev[0] = 0x1b;
		}

		newline = cli_write(ev);
		return 0;
	}
	else if(w->dim == 2)
	{
		if( (what == 0x64626b)&&(why == 0x1b) )
		{
			menu ^= 1;
			return 0;
		}

		if(menu > 0)
		{
			worker[1].write(ev);
			worker[0].write(ev);
		}
		return worker[now].write(ev);
	}
	else if(w->dim == 3)
	{
		say("dim=%d\n",w->dim);
	}
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
