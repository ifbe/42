#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void say(void*, ...);
struct piece
{
	void* buf;
	int len;
	int flag;	//0:free, 1:used
};




void memory_debug(struct piece* mem)
{
	int j = 0;
	while(1)
	{
		if(mem[j].buf == 0)break;
		say("%llx,%x,%x\n",mem[j].buf, mem[j].len, mem[j].flag);
		j++;
	}
}
void memory_shiftr(struct piece* mem, int j)
{
	int k = j;
	while(mem[k].buf != 0)k++;
	for(;k>j;k--)
	{
		mem[k].buf = mem[k-1].buf;
		mem[k].len = mem[k-1].len;
		mem[k].flag = mem[k-1].flag;
	}
}
void memory_shiftl(struct piece* mem, int j)
{
	while(1)
	{
		mem[j].buf = mem[j+1].buf;
		if(mem[j].buf == 0)break;

		mem[j].len = mem[j+1].len;
		mem[j].flag = mem[j+1].flag;
	}
}




void* memory_allocate(struct piece* mem, int len)
{
	int j=0,k=0;
	while(1)
	{
		if(mem[j].buf == 0)break;
		if(mem[j].flag == 0)
		{
			if(mem[j].len == len)
			{
				mem[j].flag = 1;
				return mem[j].buf;
			}
			if(mem[j].len > len)
			{
				if(k == 0)k = j;
			}
		}

		j++;
	}
	if(k == 0)return 0;

	memory_shiftr(mem, k);
	mem[j+1].buf += len;
	mem[j+1].len -= len;

	mem[j].len = len;
	mem[j].flag = 1;
	return mem[j].buf;
}
void memory_recycle(struct piece* mem, void* buf)
{
	int j = 0;
	while(1)
	{
		if(mem[j].buf == 0)return;
		if(mem[j].buf == buf)
		{
			if(mem[j].flag == 0)return;
			else break;
		}
		j++;
	}

	//try merge with right
	if(mem[j+1].flag == 0)
	{
		if(buf+(mem[j].len) == mem[j+1].buf)
		{
			mem[j+1].buf = buf;
			mem[j+1].len += mem[j].len;
			memory_shiftl(mem, j);
		}
	}

	//try merge with left
	if(j == 0)return;
	if(mem[j-1].flag == 0)
	{
		if(buf == (mem[j-1].buf)+(mem[j-1].len))
		{
			mem[j-1].len += mem[j].len;
			memory_shiftl(mem, j-1);
		}
	}
}
void memory_insert(struct piece* mem, void* buf, int len)
{
	int j;
	if(mem == 0)return;
	if(buf == 0)return;

	j = 0;
	while(1)
	{
		if(mem[j].buf == 0)
		{
			mem[j].buf = buf;
			mem[j].len = len;
			mem[j].flag = 0;

			mem[j+1].buf = 0;
			break;
		}
		if(mem[j].buf > buf)
		{
			memory_shiftr(mem, j);
			mem[j].buf = buf;
			mem[j].len = len;
			mem[j].flag = 0;
			break;
		}
		j++;
	}
}
void memory_prepare(struct piece* mem)
{
	mem[0].buf = 0;
}
