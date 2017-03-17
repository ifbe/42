#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
u64 startthread(void*, void*);
void stopthread();
//
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




struct clidata
{
        u64 buf;
        u64 fmt;
        u64 w;
        u64 h;

        u64 thread;
};




void windowchange()
{
}
void windowlist()
{
}
void windowread()
{
}
void windowwrite()
{
}
void windowstart(struct clidata* win)
{
	win->fmt = 0;
	win->w = 80;
	win->h = 25;
}
void windowstop()
{
}
void windowcreate()
{
}
void windowdelete()
{
}
