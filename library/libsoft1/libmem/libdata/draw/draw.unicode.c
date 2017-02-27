#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void say(void*, ...);




struct window
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;
};
static u8* unicodetable=0;




void printunicodefromvalue(struct window* win, u8* p, int xxxx, int yyyy)
{
	int x,y;
	u16 temp;

	u32* this = (u32*)(win->buf + (xxxx + (yyyy * (win->w) ) )*4 );
	for(y=0;y<0x10;y++)
	{
		temp  = p[y*2] << 8;
		temp += p[(y*2)+1];
		for(x=0;x<0x10;x++)
		{
			if( (temp&0x8000) == 0x8000 )
			{
				this[x+(y*1024)]=0xffffffff;
			}
			temp=temp<<1;
		}
	}//for
}
void printunicode(struct window* win, u32 val, int x, int y)
{
	printunicodefromvalue(win, unicodetable+(val*0x20), x, y);
}
