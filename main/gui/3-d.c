#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


tempprint()
{
	int x,y;
	background3();
	for(y=-250;y<250;y++)
	{
		for(x=-250;x<250;x++)
		{
			if(x*x+y*y<=62500)
			{
				draw(x,y,0xffffffff);
			}
		}
	}
}
tempmessage(DWORD type,DWORD key)
{
	
}