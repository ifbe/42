#include "libsoft.h"
u64 file_check(u8* buf, int len);
int mbrclient_create(_obj* ele, void* url, int argc, u8** argv);
int mbrclient_delete(_obj* ele, void* url);
int mbrclient_attach(struct halfrel* self, struct halfrel* peer);
int mbrclient_detach(struct halfrel* self, struct halfrel* peer);
int gptclient_create(_obj* ele, void* url, int argc, u8** argv);
int gptclient_delete(_obj* ele, void* url);
int gptclient_attach(struct halfrel* self, struct halfrel* peer);
int gptclient_detach(struct halfrel* self, struct halfrel* peer);




int openreadclose(void* name, int off, void* buf, int len)
{
	_obj* obj = file_create(name, 'r');
	if(0 == obj)return 0;

	int ret = file_take(obj,0, "",off, buf,len);

	file_delete(obj);
	return ret;
}
int openwriteclose(void* name, int off, void* buf, int len)
{
	_obj* obj = file_create(name, 'w');
	if(0 == obj)return 0;

	int ret = file_give(obj,0, "",off, buf,len);

	file_delete(obj);
	return ret;
}



/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		|[want     ,     want+1m]|
		|			| [where,]    //不要
 [where, ]      |			|	     //不要
	    [---|--where,--]	     |	     //要后面
	    [---|--where,----------------|----]	//要中间
		|  [where,    ]	  |	     //全要
		|  [---where,------------|----]	//要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(设备这一小块是逻辑上的哪) -> (内存这一小块想要逻辑上的哪)
(扇区，数量，是哪) -> (内存，数量，要哪)
*/
void cleverread(
	u64 src, u64 count, u64 where,
	u8* buf, u64 len, u64 want)
{
	u8* rdi = 0;    //内存地址
	u64 rsi = 0;    //扇区号
	u64 rcx = 0;    //扇区数量

	//改rdi,rsi,rcx数值
	if(where<want)	     //3和4
	{
		rdi = buf;
		rsi = src+(want-where);
		if(where+count <= want+len)
		{
			rcx = count-(want-where);
		}
		else
		{
			rcx = len;
		}
	}
	else
	{
		rdi = buf+(where-want);
		rsi = src;
		if(where+count <= want+len)
		{
			rcx = count;
		}
		else
		{
			rcx = want+len-where;
		}
	}

/*
	say(
		"(%llx,%llx,%llx)->(%llx,%llx,%llx)\n",
		src,count,where,    buf,len,want
	);
	say(
		"rdi=%llx,rsi=%llx,rcx=%llx\n",
		rdi,rsi,rcx
	);
*/
	//readfile(0, 0, "", rsi, rdi, rcx);
}




/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		|[want     ,     want+1m]|
		|			| [where,]    //不要
 [where, ]      |			|	     //不要
	    [---|--where,--]	     |	     //要后面
	    [---|--where,----------------|----]	//要中间
		|  [where,    ]	  |	     //全要
		|  [---where,------------|----]	//要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(内存这一小块是逻辑上的哪) -> (设备这一小块想要逻辑上的哪)
(来源，数量，是哪) -> (目的，数量，要哪)
*/
void cleverwrite(
	u8* buf, u64 len, u64 where,
	u64 dst, u64 size, u64 want)
{
}




int fileauto_delete(_obj* ele)
{
	if(ele->listptr.buf0){
		memorydelete(ele->listptr.buf0);
		ele->listptr.buf0 = 0;
	}
	return 0;
}
int fileauto_create(_obj* ele, u8* url)
{
	ele->listptr.buf0 = memorycreate(0x10000, 0);
	return 0;
}
int fileauto_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int fileauto_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@fileauto_attach\n");
	_obj* ele = self->pchip;
	if(0 == ele)return 0;
	void* buf = ele->listptr.buf0;
	if(0 == buf)return 0;

	int ret;
	struct item* xxx = peer->pchip;
	if((_sys_ == xxx->tier)|(_art_ == xxx->tier)){
		_obj* obj = peer->pchip;
		ret = file_take(obj,0, "", 0, buf, 0x10000);
		if(0x10000 != ret)return -1;
	}
	else{
		if(0 == xxx->ontaking)return -1;
		ret = xxx->ontaking(xxx,peer->pfoot, 0,0, 0,0, buf, 0x10000);
		if(0x10000 != ret)return -1;
	}

	u64 type = file_check(buf, 0x10000);
	if(0 == type)return -2;

	say("filetype = %.8s\n", &type);
	if(0 == type)return 0;

	//remove myself
	fileauto_delete(ele);
	ele->type = type;

	//recreate target
	switch(type){
	case _mbr_:
		mbrclient_create(ele,0,0,0);
		mbrclient_attach(self,peer);
		break;
	case _gpt_:
		gptclient_create(ele,0,0,0);
		gptclient_attach(self,peer);
		break;
	}
	return 0;
}
