#define QWORD unsigned long long
#define DWORD unsigned int
//
void rectbody( int x1, int y1, int x2, int y2, DWORD color);
void rectframe(int x1, int y1, int x2, int y2, DWORD color);
void circlebody( int cx, int cy, int r, DWORD color);
void circleframe(int cx, int cy, int r, DWORD color);
void sectorbody( int cx, int cy, int r, int start, int end, DWORD color);
void sectorframe(int cx, int cy, int r, int start, int end, DWORD color);
//
void backgroundcolor();
void say(char*,...);





void testshape_list()
{
}
void testshape_change()
{
}
void testshape_read()
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

}
void testshape_write()
{
}
void testshape_start(QWORD size,void* addr)
{
}
void testshape_stop()
{
}
void testshape_init(char*base,char* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x6570616873;

	this[10]=(QWORD)testshape_start;
	this[11]=(QWORD)testshape_stop;
	this[12]=(QWORD)testshape_list;
	this[13]=(QWORD)testshape_change;
	this[14]=(QWORD)testshape_read;
	this[15]=(QWORD)testshape_write;
}
void testshape_kill()
{
}
