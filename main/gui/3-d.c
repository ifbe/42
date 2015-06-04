#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


tempprint()
{
	int x,y;
	for(y=0;y<200;y++)
		for(x=0;x<512;x++)
			point(x,y,0x456789ab);
}
tempmessage(DWORD type,DWORD key)
{
	
}