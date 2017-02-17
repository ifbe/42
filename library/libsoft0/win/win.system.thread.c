#include<windows.h>
HANDLE startthread(void* func, void* args)
{
	CreateThread(NULL, 0, func, args, 0, NULL);
}
void stopthread()
{
}
