#ifndef _MYAUDIO_H
#define _MYAUDIO_H




//audio
struct pcmdata
{
	int fmt;
	int chan;
	int rate;
	int count;
	short buf[0];
};
struct pcmeasy{
	struct pcmdata* mic;
	struct pcmdata* wall;
	struct pcmdata* data;
	struct pcmdata* what;
};
struct pcmlist{
	struct pcmdata** mic;
	struct pcmdata** wall;
	struct pcmdata** data;
	struct pcmdata** what;
};
struct appleaudioqueue{
	void* aqbufref[16];	//+0x80
	void* zero10ms;

	void*  aqref;
	int    aqenq;
	int    aqdeq;
};




#endif
