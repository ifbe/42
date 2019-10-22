#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <direct.h>
#include <windows.h>
#include "libboot.h"




int arg2utf8(u8* src, u8* dst)
{
	int j,k;
	u32 ret,tmp;

	j = k = 0;
	while(1)
	{
		if(src[j] < 0xa)break;
		if(src[j] < 0x80)
		{
			dst[k] = src[j];
			k++;
			j++;
			continue;
		}

		ret = *(u16*)(src+j);
		j += 2;

		tmp = 0;
		MultiByteToWideChar(
			CP_ACP, 0,
			(void*)&ret, -1,
			(void*)&tmp, 2
		);
		//printf("%x\n", tmp);

		ret = 0;
		WideCharToMultiByte(
			CP_UTF8, 0,
			(void*)&tmp, -1,
			(void*)&ret, 4,
			NULL, NULL
		);
		//printf("%x\n", ret);

		*(u32*)(dst+k) = ret;
		for(ret=0;ret<4;ret++)
		{
			if(dst[k] >= 0xa)k++;
		}
	}

	dst[k] = 0;
	return k;
}
int main(int argc, char** argv)
{
	void* all;
	void* thr;
/*
	for(j=1;j<argc;j++){
		k = arg2utf8(argv[j], tmp);
	}
	if(argc <= 1)prep();
*/
	all = pwrclkcreate(_win32_, 0, argc, argv);
	thr = workercreate(_poller_, 0, 0, 0);

	workerdelete(thr);
	pwrclkdelete(all);

	return 0;
}




/*
void death()
{
	free(rawuniverse);
}
void* birth()
{
#define __size__ 0x1001000
	int j;
	u64 temp;


	//0.chdir
	boardcreate();
	createserial();
	//_chdir("datafile/");



	//1.alloc
	rawuniverse = malloc(__size__);
	if(NULL == rawuniverse)
	{
		printf("no enough momery\n");
		exit(-1);
	}
	for(j=0;j<__size__;j++)rawuniverse[j]=0;


	//2.align
	//[0x   0,0x1001000)	->	[0x0000,0x1000000)
	//[0x 234,0x1001234)	->	[0x1000,0x1001000)
	//[0x fff,0x1001fff)	->	[0x1000,0x1001000)
	//[0x1001,0x1002001)	->	[0x1000,0x1002000)
	temp = ( (u64)rawuniverse ) & 0xfff;
	if(0 != temp)temp = 0x1000 - temp;
	return rawuniverse + temp;
}

int main(int argc, char* argv[])
{
	//before
	u64 j,k;
	u8* addr = beforedawn();

	//cmdline
	termwrite("\n@42@\n", 1);
	for(j=1;j<argc;j++)
	{
		k = arg2utf8(argv[j], addr);
		say("%.*s\n", k, addr);

		if(termwrite(addr, k) < 0)goto byebye;
	}

	//load cfg
	prep();

	//main loop
	loop();

	//after mainloop: backup world
	//if(0 == dstctx)dstctx =
	//if(0 == storectx(dstctx))dump();
	openwriteclose("universe.bin",0,addr,0x1000000);

byebye:
	//after
	afterdusk();
	return 0;
}*/