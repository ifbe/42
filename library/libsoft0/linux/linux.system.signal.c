#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define u64 unsigned long long
u64 gettime();
void eventwrite(u64,u64,u64,u64);




static u64 die=0,old=0,new=0;




static void sig_int(int a)
{
	die = old;
	old = new;
	new = gettime();

	if(new-old < 500*1000)	//0.5 s
	{
		if(new-die < 1000*1000)exit(-1);
		else eventwrite(0, 0, 0, 0);
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
