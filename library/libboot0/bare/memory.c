#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void logtoall(void*, ...);




#define ADDR_KERNEL (u64) 0x100000	//[ 1M, 16M)
//
#define ADDR_STDIN  (u64)0x1000000	//[16M, 17M)
#define ADDR_STDOUT (u64)0x1100000	//[17M, 18M)
#define ADDR_STDEV  (u64)0x1200000	//[18M, 19M)
//
#define ADDR_NODE   (u64)0x2000000	//[32M, 48M)
#define ADDR_REL    (u64)0x3000000	//[48M, 64M)
//
#define ADDR_FB     (u64)0x4000000	//[64M,128M)
static u8 bitmap[1024] = {
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,	//16m
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,	//32m
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,	//48m
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,	//64m
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,	//80m
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,	//96m
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,	//112m
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1	//128m
};




void* memory_rsvd_stdin(int* size)
{
	*size = 0x100000;
	return (void*)ADDR_STDIN;
}
void* memory_rsvd_stdout(int* size)
{
	*size = 0x100000;
	return (void*)ADDR_STDOUT;
}
void* memory_rsvd_stdev(int* size)
{
	*size = 0x100000;
	return (void*)ADDR_STDEV;
}
void* memory_rsvd_nodepool(int* size)
{
	*size = 0x100000 * 16;
	return (void*)ADDR_NODE;
}
void* memory_rsvd_relpool(int* size)
{
	*size = 0x100000 * 16;
	return (void*)ADDR_REL;
}
void* memory_rsvd_framebuffer(int* size)
{
	*size = 0x100000 * 64;
	return (void*)ADDR_FB;
}




int memory_ensure(int j, int cnt)
{
	int k;
	for(k=j;k<j+cnt;k++){
		if(bitmap[k])return 0;
	}

	for(k=0;k<cnt;k++){
		bitmap[j+k] = j;
	}
	return 1;
}




void* memory_alloc(int size)
{
	u64 j,k;
	u8* buf;

	if(size&0xfffff){
		size = size&0xfffffffffff00000;
		size += 0x100000;
	}

	k = (size+0xfffff)/0x100000;
	for(j=64;j<1024;j++)
	{
		if(memory_ensure(j, k)){
			buf = (void*)(j<<20);
			for(j=0;j<size;j++)buf[j] = 0;
			return buf;
		}
	}

	return 0;
}
void* memory_alloc_align(int size, int align)
{
	u64 j,k;
	u8* buf;

	if(size&0xfffff){
		size = size&0xfffffffffff00000;
		size += 0x100000;
	}

	k = (size+0xfffff)/0x100000;
	for(j=64;j<1024;j++)
	{
		if(memory_ensure(j, k)){
			buf = (void*)(j<<20);
			for(j=0;j<size;j++)buf[j] = 0;
			return buf;
		}
	}

	return 0;
}
int memory_free(void* addr)
{
	u64 j,k;

	j = (u64)addr;
	j >>= 20;

	//for(k=j;k<j+bitmap[j];k++)bitmap[k] = 0;
	k = bitmap[j];
	if(0 == k)return 0;

	while(bitmap[j] == k){
		bitmap[j] = 0;
		j++;
	}
	return 0;
}
void* memory_setval(u8* addr, int ch, int len)
{
	int j;
	for(j=0;j<len;j++)addr[j] = ch;
	return addr;
}
void* memory_copy(u8* addr, u8* buf, int len)
{
	int j;
	for(j=0;j<len;j++)addr[j] = buf[j];
	return addr;
}
void* memory_realloc(void* src, int len)
{
	//todo: need to know size of src
	return 0;
}