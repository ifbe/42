#include "libsoft.h"
#define SENDLOCK listu64.data0
#define RECVSTAT listu64.data1
#define RECVBYTE listu64.data2
#define _a0by_ hex32('a','0','b','y')
#define _a0to_ hex32('a','0','t','o')
#define _v0by_ hex32('v','0','b','y')
#define _v0to_ hex32('v','0','t','o')
int sleep_us(int);
int hexstr2data(void*,void*);




int mediamux_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int mediamux_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int ret;
	u8 tmp[32];
	say("@mediamux:%.4s\n", &foot);

	switch(stack[sp-1].foottype){
	case _a0by_:{
		while(__sync_lock_test_and_set(&art->SENDLOCK,1))sleep_us(10);

		ret = mysnprintf(tmp,32, "a%x\n",len);
		give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
		give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);

		__sync_lock_release(&art->SENDLOCK);
		break;
	}
	case _v0by_:{
		while(__sync_lock_test_and_set(&art->SENDLOCK,1))sleep_us(10);

		ret = mysnprintf(tmp,32, "v%x\n",len);
		give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
		give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);

		__sync_lock_release(&art->SENDLOCK);
		break;
	}
	case _src_:{
		//printmemory(buf,16);
		while(len>0){
			if(0 == art->RECVSTAT){
				switch(buf[0]){
				case 'a':art->RECVSTAT = 'a';break;
				case 'v':art->RECVSTAT = 'v';break;
				default:ret = -1;goto error;
				}

				ret = hexstr2data(buf+1, &art->RECVBYTE);
				if(ret <= 0){ret = -2;goto error;}
				if('\n' != buf[1+ret]){ret = -3;goto error;}

				buf += 2+ret;
				len -= 2+ret;
				say("@begin:%c,%x\n", art->RECVSTAT, art->RECVBYTE);
			}

			//how many
			ret = art->RECVBYTE;
			if(ret > len)ret = len;

			//send it
			if('a' == art->RECVSTAT)give_data_into_peer(art,_a0to_, stack,sp, 0,0, buf,ret);
			if('v' == art->RECVSTAT)give_data_into_peer(art,_v0to_, stack,sp, 0,0, buf,ret);

			//nomore remain?
			art->RECVBYTE -= ret;
			if(0 == art->RECVBYTE)art->RECVSTAT = 0;

			//message done?
			buf += ret;
			len -= ret;
		}//while
		break;
	}
	}
	return 0;

error:
	say("err=%d@mediamux\n",ret);
	return 0;
}
int mediamux_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mediamux_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int mediamux_delete(_obj* ele, u8* url)
{
	return 0;
}
int mediamux_create(_obj* ele, u8* url)
{
	say("@mediamux_create\n");
	ele->SENDLOCK = 0;
	ele->RECVSTAT = 0;
	return 1;
}
