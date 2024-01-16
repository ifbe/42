#define u8 unsigned char
int base64_decode(u8* o, u8* in);
void logtoall(void*, ...);




int pem2bin(u8* dst, u8* src, int off, int len)
{
	int j,k=0;
	for(j=0;j<len;j++)
	{
		//-----BEGIN xxx xxx-----
		if(	(src[j+0]=='-')&&
			(src[j+1]=='-')&&
			(src[j+2]=='B')&&
			(src[j+3]=='E') )
		{
			k++;
			if(k == off+1)break;
		}
	}
	if(j>=len)return -0x11;

	for(k=j;k<len;k++)if(src[k] == 0xa){k++;break;}
	if(k>=len)return -0x12;

	return base64_decode(dst, src+k);
}
