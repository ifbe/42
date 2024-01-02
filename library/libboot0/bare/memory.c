#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void say(void*, ...);




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




void* memoryalloc(int size, int align)
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
int memoryfree(void* addr)
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
void* memorysetup(u8* addr, int ch, int len)
{
	int j;
	for(j=0;j<len;j++)addr[j] = ch;
	return addr;
}
void* memorycopy(u8* addr, u8* buf, int len)
{
	int j;
	for(j=0;j<len;j++)addr[j] = buf[j];
	return addr;
}
