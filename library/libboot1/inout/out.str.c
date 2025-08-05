#include "libboot.h"
int myvsnprintf(u8* buf, int len, u8* fmt, __builtin_va_list arg);
int mysnprintf(u8* buf, int len, u8* fmt, ...);
//
void lowlevel_output(void*, int);
//void boardserial_write(void*,int, void*,int, void*,int);
void window_give(void*,void*, void*,int, void*,int, void*,int);




#define MAXLINE 16
static int lastline[MAXLINE];
static u8* outputqueue = 0;
static int enq = 0;
void initstdout(void* addr)
{
	int j;
	for(j=0;j<MAXLINE;j++)lastline[j] = 0;

	outputqueue = addr;
	enq = 0;
}
void freestdout()
{
}
void* getstdout()
{
	return outputqueue;
}
int getcurout()
{
	return lastline[0];
}




static void* helpserialnode = (void*)1;
static struct item* boardserialnode = 0;
static void* logfilenode = 0;
static struct item* windownode = 0;
void stdout_sethelpserial(void* node)
{
	helpserialnode = node;
}
void stdout_setboardserial(void* node)
{
	boardserialnode = node;
}
void stdout_setwindow(void* node)
{
	windownode = node;
}


//update enq, update head and tail
int havelf = 0;
void updatelinetrack(int tmp, int len)
{
	int j,k;
	havelf = 0;
	lastline[0] += len;
	for(j=lastline[1];j<lastline[0]-1;j++){
		if('\n' == outputqueue[j]){
			havelf = 1;
			for(k=MAXLINE-1;k>0;k--)lastline[k] = lastline[k-1];
			lastline[1] = j+1;
		}
	}
}


void logtomem(u8* fmt, ...)
{
	int len;
	__builtin_va_list arg;

	//temp position
	u8* ptr = outputqueue+enq;
	int tmp = enq;

	//va start
	__builtin_va_start(arg, fmt);

	//convert
	len = myvsnprintf(ptr, 0x1000, fmt, arg);

	enq += len;
	if(enq > 0x3f000)enq = 0;

	//va end
	__builtin_va_end(arg);

	//
	updatelinetrack(tmp, len);
}
int logtoall(void* fmt, ...)
{
	int j,k,len;
	__builtin_va_list arg;

	//temp position
	u8* ptr = outputqueue+enq;
	int tmp = enq;

	//va start
	__builtin_va_start(arg, fmt);

	//convert
	len = myvsnprintf(ptr, 0x1000, fmt, arg);

	enq += len;
	if(enq > 0x3f000)enq = 0;

	//va end
	__builtin_va_end(arg);

	//
	updatelinetrack(tmp, len);

//----------------write to every thing that can help debug----------------
	//write debugport
	if(helpserialnode){
		lowlevel_output(ptr, len);
	}

	if(boardserialnode){
		boardserialnode->onwriter(boardserialnode,0, 0,0, ptr, len);
	}
/*
	//write logfile
	if(logfilenode){
		logfile_write(boardserialnode,0, 0,0, ptr, len);
	}
*/
	//write screen
	if(windownode && havelf){
		for(j=1;j<MAXLINE;j++){
			window_give(
				windownode,windownode,
				0,0,
				0,-j,
				outputqueue+lastline[j], lastline[j-1]-lastline[j]
			);
		}
	}
	return 0;
}




void printbigint(u8* buf, int len)
{
	int j;
	if(len<=0)return;

	logtoall((void*)"0x");
	for(j=len-1;j>=0;j--)logtoall((void*)"%02x", buf[j]);
}
int printmmio(void* ptr, int len)
{
	u8* buf = ptr;
	if((u64)buf & 3)return 0;

	int j,k;
	for(j=0;j<len;j+=4){
		if(0 == (j&0xf))logtoall((u8*)"@%-12p ", buf+j);

		logtoall((u8*)"0x%08x", *(u32*)(buf+j));

		if((j+4)>=len)break;

		logtoall((u8*)"%c", (0xc == (j&0xf)) ? '\n' : ' ');
	}
	logtoall((u8*)"\n");
	return 0;
}
int printmemory(void* ptr, int len)
{
	u8* buf = ptr;
	u8 c;
	int j,k;
	u8 tmp[128];

	while(1)
	{
		if(len <= 0)break;
		if(len <= 16)k = len;
		else k = 16;

		j = mysnprintf(tmp, 80, (void*)"@%-12p ", buf);
		for(;j<80;j++)tmp[j]=0x20;

		for(j=0;j<k;j++)
		{
			c = ((buf[j]>>4)&0xf)+0x30;
			if(c > 0x39)c += 7;
			tmp[(3*j) + 14] = c;

			c = (buf[j]&0xf)+0x30;
			if(c > 0x39)c += 7;
			tmp[(3*j) + 15] = c;

			c = buf[j];
			if((c<0x20)|(c>=0x7f))c = '.';
			tmp[14+48+j] = c;
		}
		logtoall((void*)"%.*s\n", 14+48+16, tmp);

		buf += 16;
		len -= 16;
	}
	return 0;
}
