#include "libboot.h"
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);




int nodetree_read(void* sc, void* sf, u8* buf, int len)
{
	void* dc;
	void* df;
	struct item* chip;
	struct relation* orel;
	buf[0] = '?';
	return 1;

	chip = sc;
	if(0 == sc)return 0;

	orel = chip->orel0;
	if(0 == orel)
	{
		printmemory(buf, len);
		return 0;
	}

	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);

		if(0 == orel->dsttype)break;
		else if(_fd_  == orel->dsttype)systemread(dc, df, sc, sf, buf, len);
		else if(_art_ == orel->dsttype)arteryread(dc, df, sc, sf, buf, len);
		else if(_win_ == orel->dsttype)arenaread( dc, df, sc, sf, buf, len);
		else if(_act_ == orel->dsttype)actorread( dc, df, sc, sf, buf, len);

		orel = samesrcnextdst(orel);
	}

	return 0;
}
int nodetree_write(void* sc, void* sf, u8* buf, int len)
{
	void* dc;
	void* df;
	struct item* chip;
	struct relation* orel;

	chip = sc;
	if(0 == sc)return 0;

	orel = chip->orel0;
	if(0 == orel)
	{
		printmemory(buf, len);
		return 0;
	}

	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);

		if(0 == orel->dsttype)break;
		else if(_fd_  == orel->dsttype)systemwrite(dc, df, sc, sf, buf, len);
		else if(_art_ == orel->dsttype)arterywrite(dc, df, sc, sf, buf, len);
		else if(_win_ == orel->dsttype)arenawrite( dc, df, sc, sf, buf, len);
		else if(_act_ == orel->dsttype)actorwrite( dc, df, sc, sf, buf, len);

		orel = samesrcnextdst(orel);
	}
	return 0;
}