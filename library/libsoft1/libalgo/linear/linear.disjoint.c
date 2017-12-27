#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




int disjoint_find(int* buf, int who)
{
	if(who == buf[who])return who;
	buf[who] = disjoint_find(buf, buf[who]);
	return buf[who];
}
void disjoint_union(int* buf, int x, int y)
{
	x = disjoint_find(buf, x);
	y = disjoint_find(buf, y);
	if(x == y)return;
	buf[y] = x;
}
void disjoint_prepare(int* buf, int len)
{
	int j;
	for(j=0;j<len;j++)buf[j] = j;
}
