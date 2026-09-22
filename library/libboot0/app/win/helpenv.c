#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
void logtoall(char*, ...);




static void* entry = 0;
static void* stack = 0;
void setentryandstack(void* e, void* s)
{
	entry = e;
	stack = s;
}




void poweroff()
{
	exit(-1);
}




void* pollenv()
{
	Sleep(1);
	return 0;
}
void* waitenv()
{
	return 0;
}




void freerunenv()
{
}
void initrunenv()
{
	logtoall("entry=%p stack=%p\n", entry, stack);
}