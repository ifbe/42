void initsay(void*);
void initevent(void*);
void initrelation(void*);
void debugcreate(char* type, char* addr)
{
	initsay(addr);
	initevent(addr);
	initrelation(addr);
	//say("[0,4):createed debug\n");
}
void debugdelete()
{
	//say("[0,4):deleteing debug\n");
}
