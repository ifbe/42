#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "libsoft.h"
void sleep_us(int);




//
static _obj* g_obj = 0;
static int curid = 0;
//
struct percom{
	HANDLE hcom;

	u64 thread;
	int alive;
};




int inituart(void* addr)
{
	g_obj = addr;
}
int freeuart()
{
}
void uartalloc()
{
}




int uart_insert()
{
	return 0;
}
int uart_remove()
{
	return 0;
}
int uart_search(u8* p)
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
int uart_modify()
{
	return 0;
}




DWORD WINAPI uart_thread(_obj* oo)
{
	if(0 == oo)return 0;

	int ret;
	int enq;
	int cnt=0;
	u8 buf[0x10000];
	struct halfrel stack[0x80];

	struct percom* per = (void*)oo->priv_256b;
	while(per->alive)
	{
		ret = 0x10000 - enq;
		if(ret > 0x1000)ret = 0x1000;

		ret = ReadFile(per->hcom, buf+enq, ret, (void*)&cnt, 0);
		if( (ret > 0) && (cnt > 0) )
		{
			give_data_into_peer(oo,_dst_, stack,0, 0,0, buf+enq,cnt);

			enq = (enq + cnt)%0x10000;

			sleep_us(1000*100);
		}
	}
	return 0;
}
static int uart_designate()
{
	//designedated fakeid: [8,f]
	int id = curid;
	curid = (curid+1)%8;
	return 8 + id;
}




_obj* uart_create(char* p, int speed)
{
	//
	int ret;
	char buf[20];
	for(ret=0;ret<20;ret++)if(p[ret] < 0x20)break;
	snprintf(buf, 20, "\\\\.\\%.*s", ret, p);

	//
	HANDLE hcom = CreateFile(
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
		return 0;
	}
	else say("name=%s, hcom=%llx\n", buf, hcom);

	//
	ret = uart_designate();
	struct item* oo = &g_obj[ret];
	say("hcom=%p,obj=%p\n", hcom, oo);
	struct percom* per = (void*)oo->priv_256b;
	per->hcom = hcom;

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
	per->alive = 1;
	per->thread = threadcreate(uart_thread, oo);

	//success
	return oo;
}
int uart_delete(_obj* oo)
{
	struct percom* per = (void*)oo->priv_256b;
	HANDLE hcom = per->hcom;

	per->alive = 0;
	//wait for thread exit

	CloseHandle(hcom);
}
int uart_reader(_obj* oo,int xx, void* arg,int off, void* buf,int len)
{
	struct percom* per = (void*)oo->priv_256b;
	HANDLE hcom = per->hcom;

	u32 cnt = 0;
	int ret = ReadFile(hcom, buf, len, (void*)&cnt, 0);
	return ret;
}
int uart_writer(_obj* oo,int xx, void* arg,int off, void* buf,int len)
{
	struct percom* per = (void*)oo->priv_256b;
	HANDLE hcom = per->hcom;

	u32 cnt = 0;
	int ret = WriteFile(hcom, buf,  len, (void*)&cnt, 0);
	//say("write:ret=%d,cnt=%d,errno=%d\n", ret, cnt, GetLastError());
	return ret;
}