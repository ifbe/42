#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int websocket_write(u64, u8*, int);
int rdp_write(u64, u8*, int);
int vnc_write(u64, u8*, int);
int count_strlen(void*);
//
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);
//
void printmemory(void*, int);
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
};
static struct window* win=0;
static int user[10];




int netwinread()
{
	return 0;
}
int netwinwrite(struct window* win)
{
	void* buf;
	int j,k,len;

	buf = (void*)win->buf1;
	len = count_strlen(buf);
	for(j=0;j<10;j++)
	{
		if(user[j] == 0)continue;

		k = websocket_write(user[j], buf, len);
		if(k <= 0)user[j] = 0;
	}

	return 0;
}
int netwinlist()
{
	return 0;
}
int netwinchoose(int j)
{
	return 0;
}
int netwinstop(int fd)
{
	int j;
	for(j=0;j<10;j++)
	{
		if(user[j] == fd)
		{
			user[j] = 0;
			break;
		}
	}
	return 0;
}
int netwinstart(int fd)
{
	int j;
	for(j=0;j<10;j++)
	{
		if(user[j] != 0)
		{
			if(user[j] == fd)break;	//already in
			else continue;		//another one
		}

		user[j] = fd;
		break;
	}
	return 0;
}
int netwindelete()
{
	return 0;
}
int netwincreate(void* uibase, void* uithis)
{
	int j;
	for(j=0;j<10;j++)user[j] = 0;

	win = uithis;
	win->fmt = 0x6c6d7468;
	win->w = 512;
	win->h = 512;

	return 0;
}
