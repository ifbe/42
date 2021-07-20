#include "libsoft.h"




int fuckgfwclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int fuckgfwclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@fuckgfwclient_write: %llx, %.4s, %d\n", self->pchip, &self->flag, len);
	printmemory(buf, len<16?len:16);

	return 0;
}
int fuckgfwclient_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@fuckgfwclient_discon: %.4s\n", &self->flag);
	return 0;
}
int fuckgfwclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@fuckgfwclient_linkup: %.4s\n", &self->flag);
	return 0;
}
int fuckgfwclient_delete(_obj* ele)
{
	return 0;
}
int fuckgfwclient_create(_obj* ele, u8* url)
{
	say("@fuckgfwclient_create\n");
	return 0;
}




int fuckgfwserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int fuckgfwserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	_obj* ele = self->pchip;
	if(_src_ == self->flag){
/*
		//type, id = decrypt(buf, len)
		type, id, buf,len = decrypt(buf,len)

		//connect or transmit
		if(connect == type){
			website = systemcreate();
			relationcreate(ele,id, website, _dst_);
		}
		if(message == type){
			give_data_into_peer(ele,id, arg,idx, buf,len);
		}
*/
	}
	else{
		//buf,len = encrypt(id, buf,len)
		//give_data_into_peer(ele, _src_, arg, idx, buf, len);
	}
	return 0;
}
int fuckgfwserver_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@fuckgfwserver_discon: %.4s\n", &self->flag);
	return 0;
}
int fuckgfwserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@fuckgfwserver_linkup: %.4s\n", &self->flag);
	return 0;
}
int fuckgfwserver_delete(_obj* ele)
{
	return 0;
}
int fuckgfwserver_create(_obj* ele, u8* url)
{
	say("@fuckgfwserver_create\n");
	ele->listu64.data0 = 0;
	return 0;
}




int fuckgfwmaster_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int fuckgfwmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	_obj* TCP;		//parent
	_obj* Tcp;		//child

	_obj* FUCKGFW;	//master
	_obj* Fuckgfw;	//server

	//parent, child
	TCP = (void*)(peer->chip);
	if(0 == TCP)return 0;
	Tcp = TCP->sockinfo.child;
	if(0 == Tcp)return 0;

	//master, servant
	FUCKGFW = self->pchip;
	if(0 == FUCKGFW)return 0;
	Fuckgfw = arterycreate(_Fuckgfw_, 0, 0, 0);
	if(0 == Fuckgfw)return 0;

	//child -> servant
	relationcreate(Fuckgfw, 0, _art_, _src_, Tcp, 0, _sys_, _dst_);
	return 0;
}
int fuckgfwmaster_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int fuckgfwmaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@fuckgfwmaster_linkup\n");
	return 0;
}
int fuckgfwmaster_delete(_obj* ele)
{
	return 0;
}
int fuckgfwmaster_create(_obj* ele, u8* url)
{
	say("@fuckgfwmaster\n");
	return 0;
}
