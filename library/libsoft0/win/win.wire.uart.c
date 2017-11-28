#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
u64 startthread(void*, void*);
void stopthread();
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




struct uartinfo
{
	char* buf;
	int len;
	int enq;
	int deq;
};
static struct uartinfo info;
static HANDLE hcom = 0;
static u64 thread = 0;
static int alive = 0;




DWORD WINAPI systemuart_read(LPVOID pM)
{
	int ret;
	int max;
	int count=0;

	while(alive == 1)
	{
		max = 0x100000 - (info.enq);
		if(max > 256)max = 256;

		ret = ReadFile(
			hcom,
			(info.buf)+(info.enq),
			max,
			(void*)&count,
			0
		);

		if( (ret > 0) && (count > 0) )
		{
			//printf("from %d to %d\n", info.enq, (info.enq + count)%0x200);
			info.enq = (info.enq + count)%0x100000;
			eventwrite(
				(u64)&info,
				hex32('u','a','r','t'),
				0,
				0
			);
		}

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
	//say("write:ret=%d,count=%d,errno=%d\n", ret, count, GetLastError());
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
			say("%s\n", buf+4);
			CloseHandle(h);
		}
	}
}
int systemuart_choose(char* p, int speed)
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
	thread = startthread(systemuart_read, 0);

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

