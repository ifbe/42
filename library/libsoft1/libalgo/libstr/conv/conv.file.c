#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




/*
"1.txt" -> "txt"
"2.html" -> "html"
*/
u8* getsuffix(u8* p)
{
	int j,k=0;
	if(0 == p)return 0;

	for(j=0;j<0x1000;j++)
	{
		if(p[j] < 0x20)break;
		if(p[j] == '.')k = j;
	}
	if(0 == j)return 0;
	if(0 == k)return 0;
	return p+k+1;
}




u8* getfilename(u8* p)
{
	int j=0;
	int k=0;
	while(1)
	{
		if(p[j] == 0)break;
		if(p[j] == '/')k=j+1;

		j++;
	}
	return p+k;
}




int getfolder(u8* p)
{
	int j = 0;
	int k = -1;
	while(1)
	{
		if(j>0x1000)break;
		if(p[j] == 0)break;
		if(p[j] == '/')k=j;
		j++;
	}

	if(k < 0)return 0;
	if(k == 0)return 1;
	return k;
}
