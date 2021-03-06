#include "libsoft.h"
u32 resolvehostname(void*);




int udptrav_memory(u64* list, u64 self)
{
	int j;
	if(list[0] == self)return 0;
	if(0 == list[0]){
		list[0] = self;
		return 0;
	}

	for(j=0;j<3;j++){
		if(list[j] == self)break;
	}
	for(;j>0;j--)list[j] = list[j-1];
	list[0] = self;
	return 0;
}




int udptravclient_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int udptravclient_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	//say("@udptravclient_write: %.4s\n", &foot);

	if(_std_ == stack[sp-1].flag){
		if(' ' == buf[0])give_data_into_peer(art,_src_, stack,sp, &art->data0,0, buf, 1);
		else if(art->data1)give_data_into_peer(art,_src_, stack,sp, &art->data1,0, buf, 1);
		return 0;
	}

	//p=this, s=server, f=friend
	u16 p_port = *(u16*)(arg+2);
	u32 p_addr = *(u32*)(arg+4);
	void* s = (void*)&art->data0;
	u16 s_port = *(u16*)(s+2);
	u32 s_addr = *(u32*)(s+4);
	void* f = (void*)&art->data1;
	u16 f_port = *(u16*)(f+2);
	u32 f_addr = *(u32*)(f+4);

	//from server
	if( (p_port == s_port) && (p_addr == s_addr) ) {
		u8* t;
		t = arg;
		say("server: %d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
		t = buf;
		say("myself: %d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);

		if(len < 16)return 0;
		t = buf+8;
		say("friend: %d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);

		art->data1 = *(u64*)(buf+8);
		return 0;
	}

	//from friend
	if( (p_port == f_port) && (p_addr == f_addr) ) {
		u8* t = arg;
		say("from %d.%d.%d.%d:%d->\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
		printmemory(buf, len);
		return 0;
	}

	//from unknown
	say("from unknown:\n");
	printmemory(buf,len);
	return 0;
}
int udptravclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int udptravclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@udptravclient_linkup: %.4s\n", &self->flag);
	return 0;
}
int udptravclient_delete(struct artery* art)
{
	return 0;
}
int udptravclient_create(struct artery* art, u8* url)
{
	u8* tmp = (void*)&art->data0;
	tmp[2] = 9999>>8;
	tmp[3] = 9999&0xff;
	*(u32*)(tmp+4) = resolvehostname(url);
	printmemory(tmp, 8);

	art->data1 = 0;
	return 0;
}




int udptravserver_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int udptravserver_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int udptravserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int udptravserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int udptravserver_delete(struct artery* art)
{
	return 0;
}
int udptravserver_create(struct artery* art, u8* url)
{
	return 0;
}




int udptravmaster_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int udptravmaster_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	if(0==stack|sp<2)return 0;
	struct sysobj* sys = stack[sp-2].pchip;

	if( (_UDP_ == sys->type) | (_udp_ == sys->type) ) {
		if(0 == arg){
			say("error@udptravmaster_write\n");
			return 0;
		}

		u8* t = arg;
		say("from %d.%d.%d.%d:%d->\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
		printmemory(buf, len);

		u64* list = &art->data0;
		udptrav_memory(list, *(u64*)arg);

		int j;
		for(j=0;j<4;j++){
			if(0 == list[j])break;
		}
		give_data_into_peer(art,stack[sp-1].flag, stack,sp, arg,idx, list,j*8);
	}
	return 0;
}
int udptravmaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@udptravmaster_linkup: %.4s\n", &self->flag);
	return 0;
}
int udptravmaster_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@udptravmaster_discon: %.4s\n", &self->flag);
	return 0;
}
int udptravmaster_delete(struct artery* art)
{
	return 0;
}
int udptravmaster_create(struct artery* art, u8* url)
{
	int j;
	u64* list = &art->data0;
	for(j=0;j<4;j++)list[j] = 0;
	return 0;
}
