#include "libsoft.h"
u32 resolvehostname4(void*);


struct serverresponse{
	u64 publicaddr_yourself;
	u64 publicaddr_friend[1];
};


struct udptravclient_data{
	u64 myname;
	u64 frienename;

	u64 serveraddr;		//public network

	u64 myaddr_myselfview;		//local addr by getsockname
	u64 myaddr_serverview;		//my addr will be translated after NAT

	u64 friendaddr;
};
int udptravclient_read(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int udptravclient_write(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	//logtoall("@udptravclient_write: %.4s\n", &foot);

	struct udptravclient_data* priv = (void*)art->priv_256b;

	//frome stdin
	if(_std_ == stack[sp-1].foottype){
		logtoall("from stdin:%.8s\n", buf);
		if(' ' == buf[0]){
			logtoall("sento server\n");
			give_data_into_peer(art,_src_, stack,sp, 0,0, buf, 1);
		}
		else if(priv->friendaddr){
			logtoall("sento friend\n");
			give_data_into_peer(art,_src_, stack,sp, (p64)&priv->friendaddr,0, buf, 1);
		}
		return 0;
	}

	//
	u16 inport = *(u16*)(arg+2);
	u32 inaddr = *(u32*)(arg+4);

	//from friend
	u8* f = (u8*)&priv->friendaddr;
	u16 f_port = *(u16*)(f+2);
	u32 f_addr = *(u32*)(f+4);
	if( (inport == f_port) && (inaddr == f_addr) ) {
		logtoall("from friend:\n");
		u8* t = (u8*)arg;
		logtoall("from %d.%d.%d.%d:%d->\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
		printmemory(buf, len);
		return 0;
	}

	//from server
	u8* s = (u8*)&priv->serveraddr;
	u16 s_port = *(u16*)(s+2);
	u32 s_addr = *(u32*)(s+4);
	//if( (inport == s_port) && (inaddr == s_addr) ) {
		logtoall("from server:\n");
		u8* t = (u8*)arg;
		logtoall("server: %d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
		t = buf;
		logtoall("myself: %d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);

		if(len < 16)return 0;
		t = buf+8;
		logtoall("friend: %d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);

		priv->friendaddr = *(u64*)(buf+8);
		return 0;
	//}

	//from unknown
	//logtoall("from unknown:\n");
	//printmemory(buf,len);
	return 0;
}
int udptravclient_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int udptravclient_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@udptravclient_attach: %.4s\n", &self->foottype);
	//at this time, get self socket addr
	//struct udptravclient_data* priv = (void*)art->priv_256b;
	return 0;
}
int udptravclient_delete(_obj* art)
{
	return 0;
}
int udptravclient_create(_obj* art, u8* url)
{
	struct udptravclient_data* priv = (void*)art->priv_256b;
	return 0;
}




int udptravserver_read(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int udptravserver_write(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int udptravserver_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int udptravserver_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int udptravserver_delete(_obj* art)
{
	return 0;
}
int udptravserver_create(_obj* art, u8* url)
{
	return 0;
}




struct udptravmaster_data{
	u64 list[0];
};
int udptrav_insert(u64* list, u64 addr)
{
	int j;
	for(j=0;j<8;j++){
		if(addr == list[j])break;
		if(0 == list[j]){
			list[j] = addr;
			break;
		}
	}
	return 0;
}
int udptrav_remove(u64* list, u64 addr)
{
	int j;
	for(j=0;j<8;j++){
		if(addr == list[j]){
			list[j] = 0;
			break;
		}
	}
	return 0;
}
u64 udptrav_findfriend(u64* list, u64 addr)
{
	int j;
	for(j=0;j<8;j++){
		if(0 == list[j])continue;
		if(addr == list[j])continue;
		return list[j];
	}
	return 0;
}
void udptrav_printlist(u64* list)
{
	logtoall("printlist:");
	int j;
	for(j=0;j<8;j++){
		if(0 == list[j])continue;
		logtoall("%d:%llx\n", j, list[j]);
	}
}




int udptravmaster_read(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int udptravmaster_write(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	if(0==stack|sp<2)return 0;
	_obj* sys = stack[sp-2].pchip;

	if( (_UDP_ == sys->type) | (_udp_ == sys->type) ) {
		if(0 == arg){
			logtoall("error@udptravmaster_write\n");
			return 0;
		}

		u64 thisaddr = *(u64*)arg;
		u8* t = (void*)arg;
		logtoall("from %d.%d.%d.%d:%d: \n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
		//printmemory(buf, len);

		struct udptravmaster_data* priv = (void*)art->priv_256b;
		udptrav_insert(priv->list, thisaddr);
		udptrav_printlist(priv->list);

		u64 friendaddr = udptrav_findfriend(priv->list, thisaddr);
		struct serverresponse resp;
		resp.publicaddr_yourself = thisaddr;
		resp.publicaddr_friend[0] = friendaddr;
		give_data_into_peer(art,stack[sp-1].foottype, stack,sp, arg,idx, &resp, sizeof(struct serverresponse));
	}
	return 0;
}
int udptravmaster_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@udptravmaster_attach: %.4s\n", &self->foottype);
	return 0;
}
int udptravmaster_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@udptravmaster_detach: %.4s\n", &self->foottype);
	return 0;
}
int udptravmaster_delete(_obj* art)
{
	return 0;
}
int udptravmaster_create(_obj* art, u8* url)
{
	int j;
	struct udptravmaster_data* priv = (void*)art->priv_256b;
	for(j=0;j<4;j++)priv->list[j] = 0;
	return 0;
}
