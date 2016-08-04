#define QWORD unsigned long long
#define DWORD unsigned int
//
void background_start(QWORD size,void* addr);
void shape_start(QWORD size,void* addr);
void ascii_start(QWORD size,void* addr);
void unicode_start(QWORD size,void* addr);
//
void circlebody(DWORD,DWORD,DWORD);
void circleframe(DWORD,DWORD,DWORD);
void rectbody(DWORD,DWORD,DWORD);
void rectframe(DWORD,DWORD,DWORD);
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

	circlebody(125+(125<<16), 125, 0xff);
	circleframe(375+(125<<16), 125, 0xff0000);

	rectbody(250+(250<<16), 500+(500<<16), 0xff00);
	rectframe(0+(250<<16), 250+(500<<16), 0xff00ff);
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
