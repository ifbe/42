#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "libsoft.h"
#define _uart_ hex32('u','a','r','t')
void sleep_us(int);
void systemwrite_dispatch(void*, void*, void*, int);




//
static struct object* obj = 0;
static HANDLE hcom = 0;
static int alive = 0;




DWORD WINAPI systemuart_thread(struct object* oo)
{
	int ret;
	int enq;
	int cnt=0;
	u8 buf[0x10000];
	if(0 == oo)return 0;

	while(alive == 1)
	{
		ret = 0x10000 - enq;
		if(ret > 0x1000)ret = 0x1000;

		ret = ReadFile(hcom, buf+enq, ret, (void*)&cnt, 0);
		if( (ret > 0) && (cnt > 0) )
		{
			systemwrite_dispatch(oo, 0, buf+enq, cnt);

			enq = (enq + cnt)%0x10000;

			sleep_us(1000*100);
		}
	}
	return 0;
}




int readuart(int fd, int off, void* buf, int len)
{
	return 0;
}
int writeuart(int fd, int off, void* buf, int len)
{
	u32 cnt=0;
	int ret;

	ret = WriteFile(
		hcom,
		buf,
		len,
		(void*)&cnt,
		0
	);
	//say("write:ret=%d,cnt=%d,errno=%d\n", ret, cnt, GetLastError());
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
	for(ret=0;ret<20;ret++)if(p[ret] < 0x20)break;
	snprintf(buf, 20, "\\\\.\\%.*s", ret, p);
	hcom = CreateFile(
		buf,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	say("name=%s, hcom=%llx\n", buf, hcom);
	if(hcom == INVALID_HANDLE_VALUE)
	{
		say("err:%d@open\n",GetLastError());
		return -1;
	}

	//
	COMMTIMEOUTS timeouts;
	ret = GetCommTimeouts(hcom, &timeouts);
	say("GetCommTimeouts:%d\n", ret);
	timeouts.ReadIntervalTimeout = 0xffffffff;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
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
	alive = 1;
	threadcreate(systemuart_thread, &obj[1]);

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
