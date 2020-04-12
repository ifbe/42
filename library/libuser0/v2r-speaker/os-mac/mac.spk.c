#include "libuser.h"




int speakerread(_sup* spk,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	if(spk->orel0)relationread(spk,_ctx_, stack,sp, 0,0, 0,0);

	struct pcmdata* pcm = spk->pcmdata;
	if(0 == pcm)return 0;

	say("fmt=%x,chan=%d,rate=%d,count=%d,buf=%llx\n", pcm->fmt, pcm->chan, pcm->rate, pcm->count, pcm->buf);
	return 0;
}
int speakerwrite(_sup* spk,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
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
