#define QWORD unsigned long long
#define DWORD unsigned int
extern char* _binary_unicode_unicode_start;
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
	unsigned char* p=(unsigned char*)&_binary_unicode_unicode_start;
	palette=whereispalette();
	printunicodefromvalue( x , y , p+(value*0x20) );
}




void printunicodefromvaluebig(int xxxx,int yyyy,char* p)
{
        int x,y,i,j;
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
				/*
				screen[ (x*4) +   (y*4096) ]=0xffffffff;
				screen[ (x*4+1) + (y*4096) ]=0xffffffff;
				screen[ (x*4+2) + (y*4096) ]=0xffffffff;
				screen[ (x*4+3) + (y*4096) ]=0xffffffff;
				screen[ (x*4) +   (y*4096+1024) ]=0xffffffff;
				screen[ (x*4+1) + (y*4096+1024) ]=0xffffffff;
				screen[ (x*4+2) + (y*4096+1024) ]=0xffffffff;
				screen[ (x*4+3) + (y*4096+1024) ]=0xffffffff;
				screen[ (x*4) +   (y*4096+2048) ]=0xffffffff;
				screen[ (x*4+1) + (y*4096+2048) ]=0xffffffff;
				screen[ (x*4+2) + (y*4096+2048) ]=0xffffffff;
				screen[ (x*4+3) + (y*4096+2048) ]=0xffffffff;
				screen[ (x*4) +   (y*4096+3072) ]=0xffffffff;
				screen[ (x*4+1) + (y*4096+3072) ]=0xffffffff;
				screen[ (x*4+2) + (y*4096+3072) ]=0xffffffff;
				screen[ (x*4+3) + (y*4096+3072) ]=0xffffffff;
				*/

				for(i=0;i<16;i++)
				{
				for(j=0;j<16;j++)
				{
					screen[ (y*1024*16)+(i*1024) + (x*16+j) ]=0xffffffff;
				}//forj
				}//fori
			}//forx

                        temp=temp<<1;
                }
        }//for
}
void printunicodebig(int x,int y,DWORD value)
{
	unsigned char* p=(unsigned char*)&_binary_unicode_unicode_start;
	palette=whereispalette();
	printunicodefromvaluebig( x , y , p+(value*0x20) );
}




unsigned char* whereisunicodetable()
{
        return (unsigned char*)&_binary_unicode_unicode_start;
}
