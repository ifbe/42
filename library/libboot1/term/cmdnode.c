#include "libboot.h"
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);
int mysnprintf(void*, int, void*, ...);




//leaf -> root: example: tcpip data pack until ethernet layer
int nodetree_leafread(void* sc, void* sf, u8* buf, int len)
{
	struct item* chip;
	struct relation* irel;

	chip = sc;
	if(0 == chip)return 0;

	irel = chip->irel0;
	if(0 == irel)
	{
		printmemory(buf, len);
		return 0;
	}

	return 0;
}




int nodetree_leafwrite(void* sc, void* sf, u8* buf, int len)
{
	return 0;
}




//root -> leaf: traverse read whole tree
//stack[-n]: caller's caller's caller's caller...
//stack[-1]: caller
//stack[ 0]: root
//stack[+1]: target
//stack[+n]: target's target's target's target...
int nodetree_rootread(void* sc, void* sf, u8* buf, int len)
{
	int j;
	struct relation* orel;
	struct halfrel* self;
	struct halfrel* peer;

	struct item* chip;
	struct item* dc;
	void* df;

	chip = sc;
	if(0 == sc)return 0;

	//report self
	orel = chip->orel0;
	if(0 == orel)
	{
		return mysnprintf(buf, 0x10000,
			"%.8s,%.8s,%.8s,%.8s",
			&chip->tier, &chip->type, &chip->fmt, &chip->name
		);
	}

	//search name
	//j = mysnprintf(buf, 0x10000, "%.8s,%.8s{\n", &chip->tier, &chip->type);
	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);
		self = (void*)&orel->dstchip;
		peer = (void*)&orel->srcchip;
		//j += mysnprintf(buf+j, 0x10000-j, "%.8s,%.8s\n", &dc->tier, &dc->type);

		if(0 == orel->dsttype)break;
		else if(_fd_  == orel->dsttype)system_rootread(dc, df, sc, sf, buf, len);
		else if(_art_ == orel->dsttype)artery_rootread(dc, df, sc, sf, buf, len);
		else if(_win_ == orel->dsttype)arenaread(self, peer, buf, len);
		else if(_act_ == orel->dsttype)actorread(self, peer, buf, len);

		orel = samesrcnextdst(orel);
	}

	//return j+mysnprintf(buf+j, 0x10000-j, "}");
	return 0;
}




//root -> leaf, throw data deeper into the tree
int nodetree_rootwrite(void* sc, void* sf, u8* buf, int len)
{
	struct relation* orel;
	struct halfrel* self;
	struct halfrel* peer;

	struct item* chip;
	void* dc;
	void* df;

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
		self = (void*)&orel->dstchip;
		peer = (void*)&orel->srcchip;

		if(0 == orel->dsttype)break;
		else if(_fd_  == orel->dsttype)system_rootwrite(dc, df, sc, sf, buf, len);
		else if(_art_ == orel->dsttype)artery_rootwrite(dc, df, sc, sf, buf, len);
		else if(_win_ == orel->dsttype)arenawrite(self, peer, buf, len);
		else if(_act_ == orel->dsttype)actorwrite(self, peer, buf, len);

		orel = samesrcnextdst(orel);
	}
	return 0;
}




void node(u8* buf, int len)
{
	int j,k;
	u8* argv[8];
	u8 tmp[0x1000];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	k = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(k < 2)return;

	for(j=0;j<k;j++)say("%s\n", argv[j]);
}
