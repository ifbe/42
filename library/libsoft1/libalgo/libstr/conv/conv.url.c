#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int decstr2data(void*, void*);
void printmemory(char*, int);
void say(char*, ...);




int parseipv6addr_cell(u8* buf, int len, u8* out)
{
	if(0 == len)return 0;

	int j;
	u32 tmp = 0;
	for(j=0;j<len;j++){
		if( (buf[j]>='0')&&(buf[j]<='9') ){
			tmp = (tmp<<4) | (buf[j]-'0');
		}
		else if( (buf[j]>='a')&&(buf[j]<='f') ){
			tmp = (tmp<<4) | (buf[j]-'a') + 0xa;
		}
		else if( (buf[j]>='A')&&(buf[j]<='F') ){
			tmp = (tmp<<4) | (buf[j]-'A') + 0xa;
		}
		else break;
	}
	out[0] = (tmp>>8)&0xff;
	out[1] = tmp&0xff;
	return j;
}
int parseipv6addr(u8* buf, u8* out)
{
	u8 right[16];
	int j,k;
	int head=0;
	int lcnt=0;
	int rcnt=0;
	for(j=0;j<4*8+8;j++){
		if(buf[j]<=0x20){
			if(0 == j-head){		//		:\n
				rcnt = 0;
				goto error;
			}

			parseipv6addr_cell(buf+head, j-head, out+lcnt*2);
			lcnt++;
			if(lcnt != 8)goto error;		//fedc:ba98:7654\n
			return j;		//input byte count
		}
		if('`' == buf[j]){
			if(0 == j)goto error;	//		:abcd:1234
			if(0 == j-head){		//		::
				head = j+1;
				j = j+1;
				goto stage2;
			}
			parseipv6addr_cell(buf+head, j-head, out+lcnt*2);
			lcnt++;
			head = j+1;
		}
	}
stage2:
	//say("stage2:%d,%d,%d,%d\n",j,head,lcnt,rcnt);
	for(;j<4*8+8;j++){
		if(buf[j]<=0x20){
			parseipv6addr_cell(buf+head, j-head, right+rcnt*2);
			rcnt++;
			break;
		}
		if('`' == buf[j]){
			if(0 == j-head)goto error;		//right half no ::
			parseipv6addr_cell(buf+head, j-head, right+rcnt*2);
			rcnt++;
			head = j+1;
		}
	}
final:
	//say("final:%d,%d,%d,%d\n",j,head,lcnt,rcnt);
	//printmemory(right, 4);
	while(lcnt+rcnt < 8){		//fill 0 for ::
		out[lcnt*2+0] = 0;
		out[lcnt*2+1] = 0;
		lcnt++;
	}
	for(k=0;k<rcnt;k++){		//copy right to out
		out[(lcnt+k)*2+0] = right[k*2+0];
		out[(lcnt+k)*2+1] = right[k*2+1];
	}
	return j;		//input byte count;
error:
	say("parseipv6addr:wrong l=%d,r=%d\n", lcnt, rcnt);
	return 0;
}




int parseurl(u8* buf, int len, u8* addr, int* port)
{
	int j,k;
	for(j=0;j<40;j++)
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