#include "libsoft.h"

#define status_wait 0
#define status_finish 1
#define status_error 2
struct privdata{
	u8* encoded;
	//
	u8* decoded;
	int tmpcur;
	u8 lastbyte;
	u8 status;		//0=wait 1=finish 2=error
};
int slip_encode(u8* buf, int len, u8* tmp, int max)
{
	int j;
	int sz = 0;
	tmp[0] = 0xc0;	sz = 1;

	for(j=0;j<len;j++){
		if(0xc0 == tmp[sz]){
			tmp[sz+0] = 0xdb;
			tmp[sz+1] = 0xdc;
			sz += 2;;
		}
		else if(0xdb == tmp[sz]){
			tmp[sz+0] = 0xdb;
			tmp[sz+1] = 0xdd;
			sz += 2;;
		}
		else{
			tmp[sz] = buf[j];
			sz++;
		}
	}

	tmp[sz] = 0xc0;	sz++;
	return sz;
}
int slip_decode(u8* buf, int len, struct privdata* priv)
{
	priv->status = status_wait;

	int j;
	for(int j=0;j<len;j++){
		if(0xdb == priv->lastbyte){
			if(0xdc == buf[j]){
				priv->decoded[priv->tmpcur] = 0xc0;
				priv->tmpcur++;
			}
			else if(0xdd == buf[j]){
				priv->decoded[priv->tmpcur] = 0xdb;
				priv->tmpcur++;
			}
			else{	//error
				priv->lastbyte = 0;
				priv->status = status_error;
				j++;
				break;
			}
			priv->lastbyte = buf[j];
		}
		else{
			if(0xc0 == buf[j]){
				if(0 == priv->tmpcur){	//begin
					priv->lastbyte = 0;
				}
				else{		//end
					priv->lastbyte = 0xc0;
					priv->status = status_finish;
					j++;
					break;
				}
			}
			else if(0xdb == buf[j]){
				priv->lastbyte = 0xdb;
				continue;
			}
			else{
				priv->decoded[priv->tmpcur] = buf[j];
				priv->tmpcur++;
			}
		}
	}
	return j;	//how many byte eaten
}



int slip_take(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int slip_give(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	struct privdata* priv = (void*)art->priv_256b;
	int cur = priv->tmpcur;
	int ret;
	switch(stack[sp-1].foottype){
	case _src_:
		while(1){
			ret = slip_decode(buf,len, priv);
			if(ret <= 0)return 0;

			if(status_error != priv->status){
				give_data_into_peer(art,_dst_, stack,sp, 0,0, priv->decoded+cur,priv->tmpcur-cur);
			}

			if(status_finish == priv->status){
				give_data_into_peer(art,_pkg_, stack,sp, 0,0, priv->decoded,priv->tmpcur);
			}

			len -= ret;
			if(len <= 0)break;
			buf += ret;
			cur = priv->tmpcur;
		}
		break;
	case _dst_:
		//slip_decode(art, buf, len);
		break;
	case _pkg_:
		ret = slip_encode(buf,len, priv->encoded,0x1000);
		give_data_into_peer(art,_src_, stack,sp, 0,0, priv->encoded,ret);
		break;
	}
	return 0;
}
int slip_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int slip_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int slip_read(_obj* art,void* foot, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int slip_write(_obj* art,void* foot, p64 arg, int idx, u8* buf, int len)
{
	//give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,len);
	return 0;
}
int slip_delete(_obj* art)
{
	struct privdata* priv = (void*)art->priv_256b;
	if(priv->decoded){
		memoryfree(priv->decoded);
		priv->decoded = 0;
	}
	if(priv->encoded){
		memoryfree(priv->encoded);
		priv->encoded = 0;
	}
	return 0;
}
int slip_create(_obj* art, u8* url)
{
	struct privdata* priv = (void*)art->priv_256b;
	priv->decoded = memoryalloc(0x1000, 0);
	priv->encoded = memoryalloc(0x1000, 0);
	priv->lastbyte = 0;
	priv->tmpcur = 0;
	return 0;
}