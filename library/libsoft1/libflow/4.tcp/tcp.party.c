#include "libsoft.h"
void party_gettype(u8* src, u8* dst)
{
	int j;
	for(j=0;j<8;j++)dst[j] = 0;

	if(0 == src)return;
	if('/' != src[0])return;

	for(j=0;j<8;j++)
	{
		if(src[j] <= 0x20)break;
		dst[j] = src[j];
	}
}




int partyclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct artery* art = self->pchip;
	say("@partyclient_write\n");

	relationwrite(art, _src_, 0,0, "partyclient!\n", 0);
	return 0;
}
int partyclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int partyclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int partyclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct object* obj = peer->pchip;
	if( (_sys_ == obj->tier) && (_tcp_ == obj->type) ){
		struct artery* art = self->pchip;
		relationwrite(art,_src_, 0,0, "I volunteer to join\n",20);
	}
	return 0;
}
int partyclient_delete(struct artery* ele)
{
	return 0;
}
int partyclient_create(struct artery* ele, u8* url)
{
	int j,k;
	void* obj;
	u8* tmp[0x100];
/*
	obj = systemcreate(_tcp_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, 0, obj, 0, _fd_, 0);
	//if()return 0;

	k = 0;
	for(j=0;j<0xfff;j++)
	{
		if(0x20 >= url[j])break;
		if('/' == url[j])
		{
			if(0 == k)k = j;
		}
	}
	if(k)
	{
		k = mysnprintf(tmp, 0xff, "SERVE %.*s\n", j-k, url+k);
		system_leafwrite(obj, 0, ele, 0, tmp, k);
	}*/
	return 1;
}




int partyserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct artery* art = self->pchip;
	say("@partyserver_write\n");

	relationwrite(art, _src_, 0,0, "partyserver!\n", 0);
	return 0;
}
int partyserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int partyserver_delete(struct artery* ele)
{
	return 0;
}
int partyserver_create(struct artery* ele, u8* url)
{
	return 0;
}



/*
int partymaster_write(
	struct artery* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	//0. old && state=party: search, send, state=party
	//1. new && GET && /: debug
	//2. new && GET && other: search, send, state=wait
	//3. new && party: state=party
	int j, k;
	u64 type, type1;
	u8 tmp[16];

	struct relation* irel;
	u8* GET = 0;
	u8* SERVE = 0;

	//old
	if(obj->orel0)
	{
		printmemory(buf, len);
		if(0 == sty)return 0;

		irel = ele->irel0;
		while(1)
		{
			if(0 == irel)break;

			if(irel->srcchip != (u64)obj)
			{
				type = ((u64)sty) & 0xffffffffffffff00;
				type1 = (irel->dstfoot) & 0xffffffffffffff00;
				if(type == type1)
				{
					system_leafwrite((void*)(irel->srcchip), 0, ele, 0, buf, len);
				}
			}

			irel = samedstnextsrc(irel);
		}
		return 0;
	}

	//parse
	k = 0;
	for(j=0;j<=len;j++)
	{
		if((j<len)&&(0xd != buf[j])&&(0xa != buf[j]))continue;

		//say("%.*s\n", j-k, buf+k);
		if(ncmp(buf+k, "SERVE ", 6) == 0)SERVE = buf+k+6;
		else if(ncmp(buf+k, "GET ", 4) == 0)GET = buf+k+4;

		if(0xa == buf[j+1])j++;
		k = j+1;
	}

	if(SERVE)
	{
		party_gettype(SERVE, tmp);
		type = *(u64*)tmp;
		relationcreate(ele, (void*)type, _art_, 0, obj, 0, _fd_, 0);
	}
	else if(GET)
	{
		if(0 == ncmp(GET, "/ ", 2))
		{
			j = mysnprintf(buf, 0x1000, "<html><body>");

			irel = ele->irel0;
			while(1)
			{
				if(0 == irel)break;

				j += mysnprintf(buf+j, 0x1000,
					"%llx, %llx, %llx, %llx<br>",
					irel->srcchip, irel->srcfoot, irel->dstchip, irel->dstfoot
				);

				irel = samedstnextsrc(irel);
			}

			j += mysnprintf(buf+j, 0x1000, "</body></html>");

			k = mysnprintf(buf+j, 0x1000,
				"HTTP/1.1 200 OK\r\n"
				"Content-type: text/html\r\n"
				"Content-Length: %d\r\n"
				"\r\n",
				j
			);

			system_leafwrite(obj, 0, ele, 0, buf+j, k);
			system_leafwrite(obj, 0, ele, 0, buf+0, j);
		}
		else
		{
			party_gettype(GET, tmp);
			type = *(u64*)tmp;

			irel = ele->irel0;
			while(1)
			{
				if(0 == irel)break;

				if(type == irel->dstfoot)
				{
					system_leafwrite((void*)(irel->srcchip), 0, ele, 0, buf, len);
					type = type&0xffffffffffffff00;
					type |= '?';
					relationcreate(ele, (void*)type, _art_, 0, obj, 0, _fd_, 0);
					break;
				}

				irel = samedstnextsrc(irel);
			}
		}
	}
	return 0;
}*/
int partymaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct artery* art = self->pchip;
	say("@partymaster_write\n");

	struct object* sys = peer->pchip;
	if(0 == ncmp(buf, "GET ", 4)){
		u8 tmp[128];
		int ret = mysnprintf(tmp, 128,
"HTTP/1.1 200 OK\r\n"
"\r\n"
"partymaster!"
		);
		relationwrite(art, _src_, 0,0, tmp,ret);
	if(_sys_ == sys->tier)systemdelete(sys->tempobj);
	}
	else{
		printmemory(buf, len);
	}

	return 0;
}
int partymaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int partymaster_delete(struct artery* ele)
{
	return 0;
}
int partymaster_create(struct artery* ele, u8* url)
{
	return 0;
}
