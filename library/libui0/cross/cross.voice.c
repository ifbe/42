#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
struct window
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;
};
static struct window* win;




int voiceread()
{
	return 0;
}
int voicewrite(struct window* win)
{
	return 0;
}
int voicelist()
{
	return 0;
}
int voicechoose()
{
	return 0;
}
int voicestop()
{
	return 0;
}
int voicestart()
{
	return 0;
}
int voicedelete()
{
	return 0;
}
int voicecreate(void* uibase, void* uithis)
{
	win = uithis;
	win->buf = 0;
	win->fmt = 0x6563696f76;
	return 0;
}