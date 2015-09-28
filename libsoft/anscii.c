#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




int compare(unsigned char* first,unsigned char* second)
{
	int i;
	//diary("%s,%s\n",first,second);
	for(i=0;i<16;i++)
	{
		if( (first[i]==0) && (second[i]==0) )return 0;		//比较完毕，相同
		if(first[i]!=second[i])return -1;
	}
}