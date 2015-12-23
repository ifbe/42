#define QWORD unsigned long long
#define DWORD unsigned int
char* whereisunicodetable();
char* whereispalette();
char* palette=0;




void printunicodefromvalue(int xxxx,int yyyy,char* p)
{
        int x,y;
	unsigned int* screen=(unsigned int*)( palette + xxxx*4 + (yyyy*4096) );

        unsigned short temp;
        for(y=0;y<0x10;y++)
        {
                temp  = p[y*2] << 8;
                temp += p[(y*2)+1];

                for(x=0;x<0x10;x++)
                {
                        if( (temp&0x8000) == 0x8000 )
			{
				screen[x+(y*1024)]=0xffffffff;
			}

                        temp=temp<<1;
                }
        }//for
}
void printunicode(int x,int y,DWORD value)
{
	unsigned char* p=whereisunicodetable();
	palette=whereispalette();
	printunicodefromvalue( x , y , p+(value*0x20) );
}
