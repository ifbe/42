#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<sys/time.h>
#define u8 unsigned char
#define u64 unsigned long long




u64 dateread()
{
	u64 temp;
	time_t timep;
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);

	//byte7=world

	//byte6=centry, byte5=year
	temp = 1900 + (p->tm_year);
	temp = ((temp/100)<<8) + (temp%100);

	//byte4=month
	temp = (temp<<8) + 1 + (p->tm_mon);

	//byte3=day
	temp = (temp<<8) + (p->tm_mday);

	//byte2=hour
	temp = (temp<<8) + (p->tm_hour);

	//byte1=minute
	temp = (temp<<8) + (p->tm_min);

	//byte0=second
	temp = (temp<<8) + (p->tm_sec);

	//
	return temp;
}


void datewrite(u64 date)
{
}




u64 timeread_from1970_ns()
{
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return (u64)t.tv_sec*1000*1000*1000 + t.tv_nsec;
}
u64 timeread_from1970_us()
{
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return (u64)t.tv_sec*1000*1000 + t.tv_nsec/1000;
}
u64 timeread_from1970_ms()
{
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return (u64)t.tv_sec*1000 + t.tv_nsec/1000/1000;
}
u64 timeread_from1970_s()
{
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return t.tv_sec;
}


u64 timeread_ns()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (u64)t.tv_sec*1000*1000*1000 + t.tv_nsec;
}
u64 timeread_us()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (u64)t.tv_sec*1000*1000 + t.tv_nsec/1000;
}
u64 timeread_ms()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (u64)t.tv_sec*1000 + t.tv_nsec/1000/1000;
}
u64 timeread_s()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec;
}
u64 since1970()
{
	return time(0);
}


void timewrite(u64 time)
{
}




void sleep_ns(int t)
{
	//todo
}
void sleep_us(int t)
{
	usleep(t);
}
void sleep_ms(int t)
{
	usleep(t*1000);
}