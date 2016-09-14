#include<time.h>
#include<sys/time.h>
#define u64 unsigned long long

u64 gettime()
{
	struct timeval t;
	gettimeofday(&t,0);
	return (t.tv_sec)*1000*1000 + (t.tv_usec);
}
