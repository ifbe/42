#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




void inline sort_quick_swap(u8* a, u8* b)
{
	u8 tmp = *a;
	*a = *b;
	*b = tmp;
}
void sort_quick_u8(u8* buf, int len)
{
	if(len <= 1)return;
	if(len <= 2){
		if(buf[0] > buf[1])sort_quick_swap(&buf[0], &buf[1]);
		return;
	}

	int l = 0;
	int r = len-1;
	u8 pivot = buf[(l+r)/2];
	while(l <= r){
		while(l <= r && buf[r] > pivot)r--;
		while(l <= r && buf[l] < pivot)l++;
		if(l <= r){
			if(l < r)sort_quick_swap(&buf[l], &buf[r]);
			l++;
			r--;
		}
	}

	//now r<l
	sort_quick_u8(buf, r+1);
	sort_quick_u8(buf+l, len-l);
}
