void initsay();
void initevent();
void debugcreate(char* type,char* addr)
{
	initsay(addr);
	initevent(addr+0x300000);
	//say("[0,4):createed debug\n");
}
void debugdelete()
{
	//say("[0,4):deleteing debug\n");
}
