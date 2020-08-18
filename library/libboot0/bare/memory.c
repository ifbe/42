#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




static u8 bitmap[1024] = {
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,
	1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1
};
int memory_ensure(int j, int cnt)
{
	int k;
	for(k=j;k<j+cnt;j++){
		if(bitmap[k])return 0;
	}

	for(k=0;k<cnt;k++){
		bitmap[j+k] = cnt-j;
	}
	return 1;
}




void* memorycreate(int size, int cmd)
{
	u64 j,k;
	u8* buf;

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
int memorydelete(void* addr)
{
	u64 j,k;

	j = (u64)addr;
	j >>= 20;

	for(k=j;k<j+bitmap[j];k++)bitmap[k] = 0;
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
