#include <unistd.h>




void initprocess()
{
}
void freeprocess()
{
}




int processcreate(void* file, void* args)
{
/*
	pid_t pid = fork();
	if(0 > pid){
		printf("error@fork\n");
		return pid;
	}

	if(0 == pid){
		printf("@child process\n");
		exec(file);
	}
	else{
		printf("@parent process, child pid=0x%x\n",pid);
		return pid;
	}
*/
	return 0;
}
int processdelete()
{
	return 0;
}