#include "libsoft.h"




int vt100_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vt100_write(struct halfrel* self, struct halfrel* peer, u8* arg, int idx, void* buf, int len)
{
	int j;
	//say("@vt100_write: %4s\n", &self->flag);
	//ret = write(fd, "unset PROMPT_COMMAND\n", 21);
	switch(self->flag){
		case _src_:say("%.*s", len, buf);break;
		default:relationwrite(self->pchip, _src_, 0, 0, buf, len);
	}
	return 0;
}
int vt100_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vt100_start(struct halfrel* self, struct halfrel* peer)
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
