#define u64 unsigned long long
#define u32 unsigned int
//
void line(int ax, int ay, int bx, int by, u32 color);
void bezier(int ax, int ay, int bx, int by, int cx, int cy, u32 color);
void rectbody( int x1, int y1, int x2, int y2, u32 color);
void rectframe(int x1, int y1, int x2, int y2, u32 color);
void circlebody( int cx, int cy, int r, u32 color);
void circleframe(int cx, int cy, int r, u32 color);
void sectorbody( int cx, int cy, int r, int start, int end, u32 color);
void sectorframe(int cx, int cy, int r, int start, int end, u32 color);
//
void backgroundcolor();
void say(char*,...);




//
static int px=0,py=0;




void doodle_list()
{
}
void doodle_change()
{
}
void doodle_read()
{
	backgroundcolor(0);

	//rect
	rectbody(  10, 10, 90, 90, 0xff00);
	rectframe(110, 10, 190,90, 0xff00ff);

	//circle
	circlebody(  50, 150, 40, 0xff);
	circleframe(150, 150, 40, 0xff0000);

	//moon
	circlebody(50, 250, 40, 0xffff00);
	circlebody(40, 240, 40, 0);

	//taiji
	sectorbody(150, 250, 50,  90, 270, 0xffffff);
	sectorbody(150, 250, 50, 270,  90, 0);
	sectorbody(150, 225, 25,  90, 270, 0);
	sectorbody(150, 275, 25, 270,  90, 0xffffff);

	//bezier
	line(  0, 256,  px,  py, 0xffff);
	line(512, 256,  px,  py, 0xffff);
	bezier(0, 256, 512, 256, px, py, 0xffff);
}
void doodle_write(u64* who, u64* what, u64* how)
{
	if(*what == 0x2d6d)
	{
		px = (*how)&0xffff;
		py = ((*how)>>16)&0xffff;
	}
}
void doodle_start()
{
}
void doodle_stop()
{
}
void doodle_create(char* base,char* addr)
{
	u64* this=(u64*)addr;
	this[0] = 0x74736574;
	this[1] = 0x656c646f6f64;

	this[10]=(u64)doodle_start;
	this[11]=(u64)doodle_stop;
	this[12]=(u64)doodle_list;
	this[13]=(u64)doodle_change;
	this[14]=(u64)doodle_read;
	this[15]=(u64)doodle_write;
}
void doodle_delete()
{
}
