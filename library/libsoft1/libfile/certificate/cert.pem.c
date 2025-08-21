#define u8 unsigned char
int base64_decode(u8* o, u8* in);
int ncmp(void*, void*, int);
void printmemory(void*, int);
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


int pem2der(u8* src, int len, u8* dst, int* dsize)
{
	int j,ret;
	int lastend = -1;
	int strbegin = -1;
	int strend = -1;
	int base64begin = -1;
	for(j=0;j<len-16;j++){
		if('\n' == src[j]){
			lastend = j;
		}
		else if(ncmp(src+j, "--BEGIN ", 8) == 0){
			strbegin = j+8;
			/*
			if(ncmp(src+strbegin, "CERTIFICATE", 11) == 0){
				logtoall("CERTIFICATE\n");
			}
			else if(ncmp(src+strbegin, "PRIVATE KEY", 11) == 0){
				logtoall("PRIVATE KEY\n");
			}
			else logtoall("%.10s", src+strbegin);
			*/
			j = strbegin;
			while( (src[j] != '-') && (j<len-16) )j++;
			strend = j;

			while( ( (src[j] == '-') || (src[j] == '\n') ) && (j<len-16) )j++;
			base64begin = j;

			logtoall("strbein=%x strend=%x base64begin=%x\n", strbegin, strend, base64begin);
		}
		else if(ncmp(src+j, "--END ", 6) == 0){
			logtoall("[%x,%x]%.*s\n", base64begin, lastend, strend-strbegin, src+strbegin);

			ret = base64_decode(dst, src+base64begin);
			//printmemory(dst, ret);
			if(dsize)*dsize = ret;

			while( (src[j] != '\n') && (j < len) )j++;
			return j;

			//strbegin = -1;
			//base64begin = -1;
		}
	}
	return 0;
}


int der2key_recursive(u8* src, int cur, int len)
{
	logtoall("%s: %p %x %x\n", __FUNCTION__, src, cur, len);
	int j=0;
	int k=0;
	int l;
	int sz;
	while(1){
		//logtoall("j=%x\n", j);
		if(j >= len)break;
		if(src[j+1] > 0x80){
			sz = 0;
			l = src[j+1]&0x7;
			for(k=0;k<l;k++){
				sz = (sz<<8) | src[j+2+k];
			}
		}
		else{
			sz = src[j+1];
			l = 0;
		}

		logtoall("[%x,%x)\n", cur+j, cur+j+2+l+sz);
		printmemory(src+j, 2+l);
		switch(src[j]){
		case 0x30:		//SEQUENCE
		case 0x31:
		case 0xa0:
		case 0xa3:
			der2key_recursive(src+j+2+l, cur+j+2+l, sz);
			break;
		default:
			printmemory(src+j+2+l, sz);
			break;
		}

		j += 2 + l + sz;
	}
	return 0;
}
int der2key(u8* src, int len)
{
	return der2key_recursive(src, 0, len);
}