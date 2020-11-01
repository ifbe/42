#include "libsoft.h"




int vt100_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vt100_write(_art* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void* buf, int len)
{
	int j;
	//say("@vt100_write: %4s\n", &self->flag);
	//ret = write(fd, "unset PROMPT_COMMAND\n", 21);
	switch(stack[sp-1].flag){
		case _src_:say("%.*s", len, buf);break;
		default:give_data_into_peer(art,_src_, stack,sp, 0,0, buf,len);
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




int vt100_delete(struct artery* ele)
{
	return 0;
}
int vt100_create(struct artery* ele, u8* url)
{
	say("@vt100_create\n");
	return 1;
}
