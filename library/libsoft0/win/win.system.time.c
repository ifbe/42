#include<windows.h>
#define u64 unsigned long long
u64 GetTickCount64();
u64 gettime()
{
	return 1000 * GetTickCount64();
}
void sleep_us(int t)
{
	Sleep(t/1000);
}
