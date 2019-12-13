#include "libuser.h"




int speakerread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct supply* spk = self->pchip;
	if(0 == spk)return 0;
say("@speakerread\n");
	struct relation* rel = spk->orel0;
	if(0 == rel)return 0;

	if(_ent_ == rel->dsttype)entityread((void*)(rel->dst), (void*)(rel->src), 0, 0, 0, 0);
	return 0;
}
int speakerwrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int speakerstop()
{
	return 0;
}
int speakerstart()
{
	return 0;
}




int speakerlist()
{
	return 0;
}
int speakerchoose()
{
	return 0;
}
int speakerdelete(struct supply* win)
{
	return 0;
}
int speakercreate(struct supply* win)
{
	return 0;
}




void initspeaker()
{
}
void freespeaker()
{
}
