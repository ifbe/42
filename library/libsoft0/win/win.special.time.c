#include<windows.h>
#define u64 unsigned long long
u64 GetTickCount64();
u64 gettime()
{
	return GetTickCount64();
}
