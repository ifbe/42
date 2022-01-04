#include "libsoft.h"




int vt100_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vt100_write(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int idx, u8* buf, int len)
{
	int j;
	//say("@vt100_write: %4s\n", &self->flag);
	//ret = write(fd, "unset PROMPT_COMMAND\n", 21);
	switch(stack[sp-1].flag){
	case _src_:
		say("%.*s", len, buf);
		break;
	default:
		if(0 == len){
			for(len=0;len<4;len++){
				if(0 == buf[len])break;
			}
		}
		give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);
	}
	return 0;
}
int vt100_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vt100_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int vt100_delete(_obj* ele)
{
	return 0;
}
int vt100_create(_obj* ele, u8* url)
{
	say("@vt100_create\n");
	return 1;
}
