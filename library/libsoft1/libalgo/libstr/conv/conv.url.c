#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int decstr2data(void*, void*);
void printmemory(char*, int);
void say(char*, ...);




int parseurl(u8* buf, int len, u8* addr, int* port)
{
	int j,k;
	for(j=0;j<16;j++)
	{
		if(0 == buf[j])break;
		if('/' == buf[j])break;
		if(':' == buf[j])
		{
			for(k=0;k<j;k++)addr[k] = buf[k];
			addr[j] = 0;

			j++;
			j += decstr2data(buf+j, port);
			return j;
		}
	}

	for(k=0;k<j;k++)addr[k] = buf[k];
	addr[j] = 0;
	return j;
}
int parsemyandto(u8* url,int len, u8* tmp,int max,
	u8** myaddr, int* myport, u8** toaddr, int* toport)
{
	int j;
	if(0 == url)return 0;

	//left part
	*myaddr = tmp+0;
	j = parseurl(url,len, *myaddr, myport);

	//check have right part?
	for(;j<len;j++){
		if(url[j] <= 0x20)break;
		if(('-' == url[j] && '>' == url[j+1])){
			j += 2;
			goto found;
		}
	}
	return 0;

	//right part
found:
	*toaddr = tmp+max/2;
	parseurl(url+j, len-j, *toaddr, toport);
	return 0;
}




int parseuart(u8* path, int* baud, u8* url)
{
	int j;
	u64 val = 115200;

	//copy path
	for(j=0;j<256;j++){
		if(url[j] <= 0x20){
			path[j] = 0;
			goto found2;
		}
		if(',' == url[j]){
			path[j] = 0;
			break;
		}
		path[j] = url[j];
	}

	//
	url += j;
	for(j=0;j<8;j++){
		if(',' == url[j])goto found1;
	}
	return 0;

found1:
	url += j+1;
	for(j=0;j<8;j++){
		if(' ' != url[j]){
			decstr2data(url+j, &val);
			goto found2;
		}
	}
	return 0;

found2:
	*baud = val;
	return 1;
}