#include<stdio.h>
#include <unistd.h>
#include <mach/mach_time.h>
typedef unsigned long long u64;




void datewrite(u64 x)
{
}
u64 dateread()
{
	return 0;
}




void sleep_us(int t)
{
	usleep(t);
}
void timewrite(u64 x)
{
}
u64 timeread()
{
	return mach_absolute_time() / 1000;
}