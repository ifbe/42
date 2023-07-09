#include <stdio.h>
#include <unistd.h>
#include <mach/mach_time.h>
#include<time.h>
#include<sys/time.h>
typedef unsigned long long u64;




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


void datewrite(u64 x)
{
}




u64 timeread_ms()
{
	return mach_absolute_time() / 1000 / 1000;
}
u64 timeread_us()
{
	return mach_absolute_time() / 1000;
}
u64 timeread_ns()
{
	return mach_absolute_time();
}
u64 since1970()
{
	return time(0);
}


void timewrite(u64 x)
{
}


void sleep_ns(int t)
{
	usleep(t/1000);
}
void sleep_us(int t)
{
	usleep(t);
}
void sleep_ms(int t)
{
	usleep(t*1000);
}