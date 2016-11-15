#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define u64 unsigned long long
u64 gettime();
void eventwrite(u64,u64,u64,u64);




static u64 old=0,new=0;




static void sig_int(int a)
{
	old = new;
	new = gettime();

	if(new-old < 500*1000)	//0.5 s
	{
		eventwrite(0, 0, 0, 0);
	}
	else eventwrite(0x3, 0x64626b, 0, 0);
}
static void sig_tstp(int a)
{
	eventwrite(0x1a, 0x64626b, 0, 0);
}




void createsignal()
{
	signal(SIGINT, sig_int);
	signal(SIGTSTP, sig_tstp);
}
void deletesignal()
{
}
