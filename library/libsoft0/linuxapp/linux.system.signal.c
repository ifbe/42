#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define u64 unsigned long long
u64 timeread_us();
void eventwrite(u64,u64,u64,u64);




static u64 die=0,old=0,new=0;




static void sig_int(int a)
{
	die = old;
	old = new;
	new = timeread_us();

	if(new-die < 1000*1000){
		printf("ctrl+c triple!\n");
		exit(-1);
	}
	else if(new-old < 500*1000){
		printf("ctrl+c twice!\n");
		eventwrite(0, 0, 0, 0);
	}
	else{
		printf("ctrl+c once!\n");
		eventwrite(0x3, 0x72616863, 0, 0);
	}
}
static void sig_tstp(int a)
{
	eventwrite(0x1a, 0x72616863, 0, 0);
}




void freesignal()
{
}
void initsignal()
{
	signal(SIGINT, sig_int);
	signal(SIGTSTP, sig_tstp);
}