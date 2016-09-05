#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define u64 unsigned long long



void signalcreate(char* what, void* handler)
{
	if(what == 0)return;
	if(what[0] != 'c')return;

	printf("ctrl+c=%llx\n",(u64)handler);
	signal(SIGINT, handler);
}
