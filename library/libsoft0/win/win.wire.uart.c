#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "system.h"
#define _uart_ hex32('u','a','r','t')




struct uartinfo
{
	char* buf;
	int len;
	int enq;
	int deq;
};
static struct uartinfo info;
//
static struct object* obj = 0;
static HANDLE hcom = 0;
//
static u64 thread = 0;
static int alive = 0;




DWORD WINAPI systemuart_thread(LPVOID pM)
{
	int ret, count=0;
	struct relation* orel;
	u8* buf;

	while(alive == 1)
	{
		ret = 0x100000 - (info.enq);
		if(ret > 256)ret = 256;

		buf = (info.buf)+(info.enq);
		ret = ReadFile(hcom, buf, ret, (void*)&count, 0);

		if( (ret > 0) && (count > 0) )
		{
			//printf("from %d to %d\n", info.enq, (info.enq + count)%0x200);

			orel = obj[1].orel;
			while(1)
			{
				if(0 == orel)break;
				if(_act_ == orel->dsttype)
				{
					actorwrite(
						(void*)(orel->dstchip), (void*)(orel->dstfoot),
						(void*)(orel->srcchip), (void*)(orel->srcfoot),
						buf, count
					);
				}
				orel = (struct relation*)samesrcnextdst(orel);
			}

			//eventwrite((u64)&info, _uart_, 0, 0);
			info.enq = (info.enq + count)%0x100000;
		}

		Sleep(10);
	}
	return 0;
}




int readuart(int fd, int off, char* buf, int len)
{
	return 0;
}
int writeuart(int fd, int off, char* buf, int len)
{
	u32 count=0;
	int ret;

	ret = WriteFile(
		hcom,
		buf,
		len,
		(void*)&count,
		0
	);
	//say("write:ret=%d,count=%d,errno=%d\n", ret, count, GetLastError());
	return ret;
}
int listuart(u8* p)
{
	int j,k=0;
	HANDLE h;

	for(j=0;j<50;j++)
	{
		snprintf(p+k, 20, "\\\\.\\COM%d", j);
		h = CreateFile(
			p+k,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
			NULL
		);
		if(h != INVALID_HANDLE_VALUE)
		{
			CloseHandle(h);
			k += snprintf(p+k, 20, "COM%d\n", j);
		}
	}
	return k;
}
int chooseuart()
{
	return 0;
}
int stopuart()
{
}
int startuart(char* p, int speed)
{
	//
	int ret;
	char buf[20];

	//
	if(hcom != 0)
	{
		alive = 0;

		CloseHandle(hcom);
		hcom = 0;
	}
	if(p == 0)return 0;

	//
	snprintf(buf, 20, "\\\\.\\%s", p);
	hcom = CreateFile(
		buf,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	say("hcom=%x\n",hcom);
	if(hcom == INVALID_HANDLE_VALUE)
	{
		say("err:%d@open\n",GetLastError());
		return -1;
	}

	//
	COMMTIMEOUTS timeouts;
	ret = GetCommTimeouts(hcom, &timeouts);
	say("GetCommTimeouts:%d\n", ret);
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 100;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 100;
	ret = SetCommTimeouts(hcom, &timeouts);
	say("SetCommTimeouts:%d\n", ret);

	//
	DCB dcb;
	ret = GetCommState(hcom,&dcb);
	say("GetCommState:%d\n", ret);
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = speed;	//波特率
	dcb.ByteSize = 8;		//数据位
	dcb.fBinary = TRUE;		//是否允许传二进制
	dcb.Parity = NOPARITY;	//奇偶校验方式
	dcb.StopBits = ONESTOPBIT;	//停止位
	ret = SetCommState(hcom,&dcb);
	say("SetCommState:%d\n", ret);

	//
	ret = SetupComm(hcom, 0x1000, 0x1000);
	say("SetupComm:%d\n", ret);
	ret = PurgeComm(
		hcom,
		PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT
	);
	say("PurgeComm:%d\n", ret);

	//
	if(info.buf == 0)
	{
		info.enq = 0;
		info.deq = 0;
		info.len = 0x100000;
		info.buf = (char*)malloc(info.len);
	}

	//
	alive = 1;
	thread = threadcreate(systemuart_thread, 0);

	//success
	return 1;
}
int deleteuart()
{
}
int createuart(void* addr)
{
	obj = addr;
}
