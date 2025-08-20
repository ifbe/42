#define u8 unsigned char
int base64_decode(u8* o, u8* in);
int ncmp(void*, void*, int);
void printmemory(void*, int);
void logtoall(void*, ...);



int pem2der(u8* src, int len, u8* dst, int max)
{
	int j,ret;
	int lastend = -1;
	int strbegin = -1;
	int base64begin = -1;
	for(j=0;j<len-16;j++){
		if('\n' == src[j]){
			if( (strbegin > 0) && (base64begin <= 0) ){
				base64begin = j+1;
				/*
				if(ncmp(src+strbegin, "CERTIFICATE", 11) == 0){
					logtoall("CERTIFICATE\n");
				}
				else if(ncmp(src+strbegin, "PRIVATE KEY", 11) == 0){
					logtoall("PRIVATE KEY\n");
				}
				else logtoall("%.10s", src+strbegin);
				*/
			}
			lastend = j;
		}
		else if(ncmp(src+j, "--BEGIN ", 8) == 0){
			strbegin = j+8;
		}
		else if(ncmp(src+j, "--END ", 6) == 0){
			logtoall("[%x,%x]%.11s\n", base64begin, lastend, src+strbegin);

			ret = base64_decode(dst, src+base64begin);
			printmemory(dst, ret);

			strbegin = -1;
			base64begin = -1;
		}
	}
	return 0;
}

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
