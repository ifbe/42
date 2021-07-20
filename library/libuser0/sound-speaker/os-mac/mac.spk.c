#include "libuser.h"




int speaker_take(_sup* spk,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	if(spk->orel0)take_data_from_peer(spk,_ctx_, stack,sp, 0,0, 0,0);

	struct pcmdata* pcm = spk->pcmeasy_data;
	if(0 == pcm)return 0;

	say("fmt=%x,chan=%d,rate=%d,count=%d,buf=%llx\n", pcm->fmt, pcm->chan, pcm->rate, pcm->count, pcm->buf);
	return 0;
}
int speaker_give(_sup* spk,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
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
int speakerdelete(_obj* win)
{
	return 0;
}
int speakercreate(_obj* win)
{
	return 0;
}




void initspeaker()
{
}
void freespeaker()
{
}
