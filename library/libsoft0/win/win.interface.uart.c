#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#define u64 unsigned long long
#define u32 unsigned int
static int alive=0;
static HANDLE hcom=0;
static HANDLE thread=0;




DWORD WINAPI systemuart_read(LPVOID pM)
{
	int ret;
	u32 count=0;
	char buf[256];

	while(alive == 1)
	{
		ret = ReadFile(
			hcom,
			buf,
			256,
			(void*)&count,
			0
		);
		buf[count] = 0;

		printf("%s", buf);
		fflush(stdout);

		Sleep(10);
	}
	return 0;
}
int systemuart_write(char* buf, int len)
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
	//printf("write:ret=%d,count=%d,errno=%d\n", ret, count, GetLastError());
	return ret;
}




int systemuart_list()
{
	int j;
	HANDLE h;
	char buf[20];
	for(j=0;j<50;j++)
	{
		snprintf(buf, 20, "\\\\.\\COM%d", j);
		h = CreateFile(
			buf,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
			NULL
		);
		if(h != INVALID_HANDLE_VALUE)
		{
			printf("%s\n", buf+4);
			CloseHandle(h);
		}
	}
}
int systemuart_choose(char* p)
{
	//
	int ret;
	char buf[20];
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
	if(hcom == INVALID_HANDLE_VALUE)
	{
		printf("fail@open\n");
		return -1;
	}

	//
	COMMTIMEOUTS timeouts;
	ret = GetCommTimeouts(hcom, &timeouts);
	printf("GetCommTimeouts:%d\n", ret);
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 100;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 100;
	ret = SetCommTimeouts(hcom, &timeouts);
	printf("SetCommTimeouts:%d\n", ret);

	//
	DCB dcb;
	ret = GetCommState(hcom,&dcb);
	printf("GetCommState:%d\n", ret);
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = 115200;	//波特率
	dcb.ByteSize = 8;		//数据位
	dcb.fBinary = TRUE;		//是否允许传二进制
	dcb.Parity = NOPARITY;	//奇偶校验方式
	dcb.StopBits = ONESTOPBIT;	//停止位
	ret = SetCommState(hcom,&dcb);
	printf("SetCommState:%d\n", ret);

	//
	ret = SetupComm(hcom, 0x1000, 0x1000);
	printf("SetupComm:%d\n", ret);
	ret = PurgeComm(
		hcom,
		PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT
	);
	printf("PurgeComm:%d\n", ret);

	//
	alive = 1;
	thread = CreateThread(NULL, 0, systemuart_read, NULL, 0, NULL);

	//success
	return 1;
}




int systemuart_stop()
{
}
int systemuart_start(char* p)
{
}
int systemuart_create()
{
}
int systemuart_delete()
{
}

