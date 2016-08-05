#define QWORD unsigned long long
#define DWORD unsigned int
//
void background_start(QWORD size,void* addr);
void shape_start(QWORD size,void* addr);
void ascii_start(QWORD size,void* addr);
void unicode_start(QWORD size,void* addr);
//
void rectbody(DWORD,DWORD,DWORD);
void rectframe(DWORD,DWORD,DWORD);
void circlebody(DWORD,DWORD,DWORD);
void circleframe(DWORD,DWORD,DWORD);
void sectorbody(DWORD,DWORD,DWORD,DWORD);
void sectorframe(DWORD,DWORD,DWORD,DWORD);
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
	rectbody(10+(10<<16), 90+(90<<16), 0xff00);
	rectframe(110+(10<<16), 190+(90<<16), 0xff00ff);

	//circle
	circlebody(50+(150<<16), 40, 0xff);
	circleframe(150+(150<<16), 40, 0xff0000);

	//moon
	circlebody(50+(250<<16), 40, 0xffff00);
	circlebody(40+(240<<16), 40, 0);

	//taiji
	sectorbody(150+(250<<16), 50, 90+(270<<16),0xffffff);
	sectorbody(150+(250<<16), 50, 270+(90<<16),0);
	sectorbody(150+(225<<16), 25, 90+(270<<16),0);
	sectorbody(150+(275<<16), 25, 270+(90<<16),0xffffff);

}
void testshape_write()
{
}
void testshape_start(QWORD size,void* addr)
{
        ascii_start(size,addr);
        unicode_start(size,addr);
        background_start(size,addr);
        shape_start(size,addr);
}
void testshape_stop()
{
}
void testshape_init(char*base,char* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x6570616873;
	this[2]=(QWORD)testshape_start;
	this[3]=(QWORD)testshape_stop;
	this[4]=(QWORD)testshape_list;
	this[5]=(QWORD)testshape_change;
	this[6]=(QWORD)testshape_read;
	this[7]=(QWORD)testshape_write;
}
void testshape_kill()
{
}
