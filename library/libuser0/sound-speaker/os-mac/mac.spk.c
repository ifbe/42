#include "libuser.h"




int speaker_take(_obj* spk,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	if(spk->orel0)take_data_from_peer(spk,_ctx_, stack,sp, 0,0, 0,0);

	struct pcmdata* pcm = spk->pcmeasy_data;
	if(0 == pcm)return 0;

	logtoall("fmt=%x,chan=%d,rate=%d,count=%d,buf=%llx\n", pcm->fmt, pcm->chan, pcm->rate, pcm->count, pcm->buf);
	return 0;
}
int speaker_give(_obj* spk,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int speaker_attach()
{
	return 0;
}
int speaker_detach()
{
	return 0;
}




int speaker_reader(_obj* spk,void* foot, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int speaker_writer(_obj* spk,void* foot, p64 arg,int idx, void* buf, int len)
{
	return 0;
}
int speaker_delete(_obj* win)
{
	return 0;
}
int speaker_create(_obj* win)
{
	return 0;
}




void initspeaker()
{
}
void freespeaker()
{
}
