#include "arena.h"




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
int voicestart(struct window* win)
{
	win->buf = 0;
	win->fmt = 0x6563696f76;
	return 0;
}
int voicedelete()
{
	return 0;
}
int voicecreate(void* uithis)
{
	return 0;
}
