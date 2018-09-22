#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <direct.h>
#include <windows.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
void createserial();




//
static u8* rawuniverse = 0;




void* pollenv()
{
	Sleep(1);
	return 0;
}
int argv2line(u8* src, u8* dst)
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

	dst[k] = '\n';
	k++;
	dst[k] = 0;
	return k;
}




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
