#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int lang_create(void*,void*);
int lang_delete();
//
int text_create(void*,void*);
int text_delete();
//
int readfile(u64 file, u8* mem, u64 offset, u64 count);
int writefile(u64 file, u8* mem, u64 offset, u64 count);
int startfile(void*);
int stopfile(int);
//
int printmemory(void* addr, int count);
int say(void* str, ...);





int openreadclose(void* name, void* buf, int off, int len)
{
	int ret;
	int fd = startfile(name);
	if(fd <= 0)return fd;

	ret = readfile(fd, buf, off, len);

	stopfile(fd);
	return ret;
}
int openwriteclose(void* name, void* buf, int off, int len)
{
	int ret;
	int fd = startfile(name);
	if(fd <= 0)return fd;

	ret = writefile(fd, buf, off, len);

	stopfile(fd);
	return ret;
}



/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		|[want     ,     want+1m]|
		|			| [where,]    //不要
 [where, ]      |			|	     //不要
	    [---|--where,--]	     |	     //要后面
	    [---|--where,----------------|----]	//要中间
		|  [where,    ]	  |	     //全要
		|  [---where,------------|----]	//要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(设备这一小块是逻辑上的哪) -> (内存这一小块想要逻辑上的哪)
(扇区，数量，是哪) -> (内存，数量，要哪)
*/
void cleverread(
	u64 src, u64 count, u64 where,
	u8* buf, u64 len, u64 want)
{
	u8* rdi = 0;    //内存地址
	u64 rsi = 0;    //扇区号
	u64 rcx = 0;    //扇区数量

	//改rdi,rsi,rcx数值
	if(where<want)	     //3和4
	{
		rdi = buf;
		rsi = src+(want-where);
		if(where+count <= want+len)
		{
			rcx = count-(want-where);
		}
		else
		{
			rcx = len;
		}
	}
	else
	{
		rdi = buf+(where-want);
		rsi = src;
		if(where+count <= want+len)
		{
			rcx = count;
		}
		else
		{
			rcx = want+len-where;
		}
	}

/*
	say(
		"(%llx,%llx,%llx)->(%llx,%llx,%llx)\n",
		src,count,where,    buf,len,want
	);
	say(
		"rdi=%llx,rsi=%llx,rcx=%llx\n",
		rdi,rsi,rcx
	);
*/
	readfile(0, rdi, rsi, rcx);
}




/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		|[want     ,     want+1m]|
		|			| [where,]    //不要
 [where, ]      |			|	     //不要
	    [---|--where,--]	     |	     //要后面
	    [---|--where,----------------|----]	//要中间
		|  [where,    ]	  |	     //全要
		|  [---where,------------|----]	//要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(内存这一小块是逻辑上的哪) -> (设备这一小块想要逻辑上的哪)
(来源，数量，是哪) -> (目的，数量，要哪)
*/
void cleverwrite(
	u8* buf, u64 len, u64 where,
	u64 dst, u64 size, u64 want)
{
}




//
static int memory_list(u8* arg1)
{
	return 0;
}
static int memory_choose(u8* arg)
{
	return 0;
}
static int memory_read(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int memory_write(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int memory_start(u8* p)
{
	return 0;
}
static int memory_stop()
{
	return 0;
}
int memory_create(u8* base, u64* p)
{
	u8* q = (u8*)p;

	//
	p[0]=0;
	p[1]=0x79726f6d656d;
	p[2]=(u64)memory_start;
	p[3]=(u64)memory_stop;
	p[4]=(u64)memory_list;
	p[5]=(u64)memory_choose;
	p[6]=(u64)memory_read;
	p[7]=(u64)memory_write;
	q += 0x100;

	lang_create(base, q);
	q += 0x100;

	text_create(base, q);
	q += 0x100;

	return q-(u8*)p;
}
int memory_delete()
{
	text_delete();
	lang_delete();
	return 0;
}
