#include <stdio.h>
#include <stdlib.h>
#include "libboot.h"
/*
//
int sleep_us();
int termwrite(void* buf, int len);
int openwriteclose(void*,int,void*,int);
//
void birth(void* addr);
void death();
//
void prep();
void loop();


int main(int argc, char** argv)
{
	//before
	int j;
	u8* addr = malloc(0x1000000);
	birth(addr);

	//prep
	pwrclkcreate(_main_, 0, argc, argv);

	for(j=1;j<argc;j++)termwrite(argv[j], 0);
	if(argc <= 1)prep();

	//loop
	workercreate(_loop_, 0, 0, 0);

	//after
	openwriteclose("universe.bin",0,addr,0x1000000);
	free(addr);
	death();
	return 0;
}*/




void* pollenv()
{
	//sleep_us(1000);
	return 0;
}
int main(int argc, char** argv)
{
	void* all;
	void* thr;

	all = pwrclkcreate(_main_, 0, argc, argv);
	thr = workercreate(_poller_, 0, 0, 0);

	workerdelete(thr);
	pwrclkdelete(all);
	return 0;
}