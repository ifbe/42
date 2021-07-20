#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)




struct perobj{
	u8* buf;
	int len;
};




int reline_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@reline_read\n");

	float f[10];
	take_data_from_peer(art,_src_, stack,sp, 0,0, f,10);
	return 0;
}
int reline_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int j,k,cur;
	u8* tmp;
	struct perobj* perobj = (void*)art->priv_256b;
	//say("@reline_write:%.4s\n", &self->flag);

	switch(stack[sp-1].flag){
	case _dst_:
		printmemory(buf, len);
		break;
	case _src_:
		//remain part
		cur = perobj->len;
		tmp = perobj->buf;
		if(cur){
			for(j=0;j<len;j++){
				tmp[cur] = buf[j];
				cur++;

				if('\n' == buf[j]){
					//say("@reline_write:%.*s", cur, tmp);
					give_data_into_peer(art,_dst_, stack,sp, 0,0, tmp,cur-1);
					cur = 0;

					j++;
					break;
				}
			}
			perobj->len = cur;

			buf += j;
			len -= j;
		}
		if(len <= 0)return 0;

		//other part
		k = 0;
		for(j=0;j<len;j++){
			if('\n' == buf[j]){
				//say("@reline_write:%.*s", j-k+1, buf+k);
				give_data_into_peer(art,_dst_, stack,sp, 0,0, buf+k,j-k+1);
				k = j+1;
			}
		}
		if(k >= len)return 0;

		//
		for(j=0;j<len-k;j++)tmp[j] = buf[k+j];
		perobj->len = j;

		break;
	}

	return 0;
}
int reline_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reline_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int reline_create(_obj* ele, u8* url)
{
	say("@reline_create\n");

	struct perobj* perobj = (void*)ele->priv_256b;
	perobj->buf = memorycreate(0x1000, 0);
	perobj->len = 0;
	return 1;
}
