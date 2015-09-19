#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
static unsigned char* listbuf;




void list()			//ls?
{
	
}
void choose(QWORD what)		//choose?
{
	filechoose(what);
}
void readmemory(QWORD buf,QWORD sector,QWORD ignore,DWORD count)		//read?
{
	//if(this is disk or file)
	fileread(buf,sector,0,count);
}
void writememory()		//write?
{
	
}




void initlist(unsigned long long addr)
{
	listbuf=(void*)addr;
	fileinit(addr);
}
