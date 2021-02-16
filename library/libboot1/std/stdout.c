#define s8 char
#define s16 short
#define s32 int
#define s64 long long
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int myvsnprintf(u8* buf, int len, u8* fmt, __builtin_va_list arg);
int mysnprintf(u8* buf, int len, u8* fmt, ...);
void lowlevel_output(void*, int);
void window_give(void*,void*, void*,int, void*,int, void*,int);




#define MAXLINE 16
static int lastline[MAXLINE];
static u8* outputqueue;
static int enq = 0;
void initstdout(void* addr)
{
	int j;
	for(j=0;j<32;j++)lastline[j] = 0;

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




static void* serialnode = (void*)1;
static void* dbglognode = 0;
static void* windownode = 0;
void stdout_setseiral(void* node)
{
	serialnode = node;
}
void stdout_setwindow(void* node)
{
	windownode = node;
}




void dbg(u8* fmt, ...)
{
	int j,k,len;
	__builtin_va_list arg;

	//va start
	__builtin_va_start(arg, fmt);

	//convert
	len = myvsnprintf(outputqueue+enq, 0x1000, fmt, arg);

	//va end
	__builtin_va_end(arg);

	//update enq, update head and tail
	if(enq < lastline[1]){
		lastline[1] = 0;
		lastline[0] = enq;
	}

	enq += len;
	if(enq > 0x3f000)enq = 0;

	lastline[0] += len;
	for(j=lastline[1];j<lastline[0]-1;j++){
		if('\n' == outputqueue[j]){
			for(k=MAXLINE-1;k>0;k--)lastline[k] = lastline[k-1];
			lastline[1] = j+1;
		}
	}

}
void say(u8* fmt, ...)
{
	int j,k,len;
	__builtin_va_list arg;

	//temp position
	u8* ptr;
	ptr = outputqueue+enq;

	//va start
	__builtin_va_start(arg, fmt);

	//convert
	len = myvsnprintf(outputqueue+enq, 0x1000, fmt, arg);

	//va end
	__builtin_va_end(arg);

	//update enq, update head and tail
	if(enq < lastline[1]){
		lastline[1] = 0;
		lastline[0] = enq;
	}

	enq += len;
	if(enq > 0x3f000)enq = 0;

	lastline[0] += len;
	for(j=lastline[1];j<lastline[0]-1;j++){
		if('\n' == outputqueue[j]){
			for(k=MAXLINE-1;k>0;k--)lastline[k] = lastline[k-1];
			lastline[1] = j+1;
		}
	}

	//write debugport
	if(serialnode){
		lowlevel_output(ptr, len);
	}

	//write logfile
/*	if(dbglognode){
	}*/

	//write screen
	if(windownode){
		for(j=1;j<MAXLINE;j++){
			window_give(
				windownode,windownode,
				0,0,
				0,-j,
				outputqueue+lastline[j], lastline[j-1]-lastline[j]
			);
		}
	}
}




void printbigint(u8* buf, int len)
{
	int j;
	if(len<=0)return;

	say((void*)"0x");
	for(j=len-1;j>=0;j--)say((void*)"%02x", buf[j]);
}
void printmmio(u8* buf, int len)
{
	if((u64)buf & 3)return;

	int j,k;
	for(j=0;j<len;j+=4){
		if(0 == (j&0xf))say((u8*)"@%-12p ", buf+j);

		say((u8*)"0x%08x", *(u32*)(buf+j));

		if((j+4)>=len)break;

		say((u8*)"%c", (0xc == (j&0xf)) ? '\n' : ' ');
	}
	say((u8*)"\n");
}
void printmemory(u8* buf, int len)
{
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
		say((void*)"%.*s\n", 14+48+16, tmp);

		buf += 16;
		len -= 16;
	}
}
