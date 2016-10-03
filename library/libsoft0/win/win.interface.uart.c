#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#define u64 unsigned long long
#define u32 unsigned int
static HANDLE hcom=0;




int systemuart_list()
{
	int j;
	HANDLE h;
	char buf[20];
	for(j=0;j<20;j++)
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
			printf("%s\n", buf);
			CloseHandle(h);
		}
	}
}
int systemuart_choose()
{
}
int systemuart_read(char* p)
{
	u32 count=0;
	OVERLAPPED writeOS;

	memset(&writeOS, 0, sizeof(OVERLAPPED)); 
	writeOS.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	int ret = ReadFile(
		hcom,
		p,
		1000,
		(void*)&count,
		&writeOS
	);
	printf("read:ret=%d,count=%d,errno=%d\n", ret, count, GetLastError());
	return ret;
}
int systemuart_write(char* p)
{
	u32 count=0;
	OVERLAPPED writeOS;

	memset(&writeOS, 0, sizeof(OVERLAPPED)); 
	writeOS.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	int ret = WriteFile(
		hcom,
		p,
		3,
		(void*)&count,
		&writeOS
	);
	printf("write:ret=%d,count=%d,errno=%d\n", ret, count, GetLastError());
	return ret;
}
int systemuart_stop()
{
	if(hcom != 0)CloseHandle(hcom);
}
int systemuart_start(char* p)
{
	//
	int ret;
	if(p == 0)return 0;
	systemuart_stop();

	//
	hcom = CreateFile(
		p,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
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
	timeouts.ReadTotalTimeoutConstant = 1000;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
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
}
int systemuart_create()
{
}
int systemuart_delete()
{
}

