#include "libuser.h"




int speakerread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct supply* spk = self->pchip;
	if(spk->orel0)relationread(spk, _ctx_, 0, 0, 0, 0);

	struct pcmdata* pcm = spk->pcmdata;
	if(0 == pcm)return 0;

	say("fmt=%x,chan=%d,rate=%d,buf=%llx\n", pcm->fmt, pcm->chan, pcm->rate, pcm->buf);
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
