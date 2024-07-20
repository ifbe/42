#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
#define _drv_ hex32('d','r','v', 0)
#define _sock_ hex32('s','o','c','k')
//
#define _time_ hex32('t','i','m','e')
#define _volt_ hex32('v','o','l','t')
#define _v0v2_ hex32('v','0','v','2')
#define _v0v1_ hex32('v','0','v','1')
int decstr2u64(u8* src, u64* dst);
int sleep_us(int);



struct msg{
	union{
		u32 type;
		char str[4];
	};
	union{
		u8 _u8[4];
		u32 _u32;
		float _f32;
	};
};
struct perobj{
};


int vehicleclient_sock(_obj* art,void* foot, u8* buf, int len)
{
	struct msg* msg = (void*)buf;
	switch(msg->type){
	case _time_:
		logtoall("day=%d,hour=%d,minute=%d,second=%d\n", msg->_u8[3], msg->_u8[2], msg->_u8[1], msg->_u8[0]);
		break;
	case _v0v1_:
		logtoall("v0v1=%f\n", msg->_f32);
		break;
	case _v0v2_:
		logtoall("v0v2=%f\n", msg->_f32);
		break;
	}
	return 0;
}
int vehicleclient_std(_obj* art,void* foot, u8* buf, int len)
{
	u32 ch[2];
	ch[0] = _char_;
	ch[1] = *(u32*)buf;

	switch(ch[1]){
	case 0x415b1b:
		ch[1] = 'w';
		break;
	case 0x425b1b:
		ch[1] = 's';
		break;
	case 0x445b1b:
		ch[1] = 'a';
		break;
	case 0x435b1b:
		ch[1] = 'd';
		break;
	}

	logtoall("sending %x(%c)\n", ch[1], ch[1]);
	give_data_into_peer_temp_stack(art, _sock_, 0,0, ch, 8);
	return 0;
}


int vehicleclient_takeby(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vehicleclient_giveby(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	struct perobj* perobj = (void*)art->priv_256b;
	//logtoall("@vehicleclient_giveby:len=%x,foot=%.4s\n",len,&stack[sp-1].foottype);
	//printmemory(buf,len>8?8:len);

	switch(stack[sp-1].foottype){
	case _sock_:
		vehicleclient_sock(art,foot, buf,len);
		break;
	case _std_:
		vehicleclient_std(art,foot, buf,len);
		break;
	}
	return 0;
}
int vehicleclient_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vehicleclient_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int vehicleclient_read(_obj* art,void* foot, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int vehicleclient_write(_obj* art,void* foot,void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int vehicleclient_create(_obj* ele, void* arg, int argc, u8** argv)
{
	logtoall("@vehicleclient_create\n");

	struct perobj* perobj = (void*)ele->priv_256b;

	//initgpio
	//initi2c
	return 1;
}
int vehicleclient_delete(_obj* ele)
{
	return 0;
}




void vehicleserver_sock(_obj* art,void* foot, _syn* stack,int sp, void* buf,int len)
{
	u32* in = buf;
	give_data_into_peer(art,_drv_, stack,sp, 0,0, &in[1],1);
}




int vehicleserver_takeby(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vehicleserver_giveby(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	struct perobj* perobj = (void*)art->priv_256b;
	//logtoall("@vehicleserver_giveby:len=%x,foot=%.4s\n",len,&stack[sp-1].foottype);
	switch(stack[sp-1].foottype){
	case _sock_:
		vehicleserver_sock(art,0, stack,sp, buf,len);
		break;
	}
	return 0;
}
int vehicleserver_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vehicleserver_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@vehicleserver_attach\n");
	return 0;
}




static void pollerthread(_obj* ele);
int vehicleserver_read(_obj* art,void* foot, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int vehicleserver_write(_obj* art,void* foot,void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int vehicleserver_create(_obj* ele, void* arg, int argc, u8** argv)
{
	logtoall("@vehicleserver_create\n");

	struct perobj* perobj = (void*)ele->priv_256b;

	threadcreate(pollerthread, ele);
	return 1;
}
int vehicleserver_delete(_obj* ele)
{
	return 0;
}




static void pollerthread(_obj* ele)
{
	u64 time;
	float v[4];
	u8 buf[16];
	while(1){
		//1.time
		time = dateread();
		logtoall("time=%llx\n", time);

		*(u32*)(buf+0) = _time_;
		*(u32*)(buf+4) = time&0xffffffff;
		give_data_into_peer_temp_stack(ele, _sock_, 0,0, buf, 8);

		//2.volt
		take_data_from_peer_temp_stack(ele, _volt_, 0,0, v, 4);

		//v[0] *= 1.0;
		v[1] *= 2.0;
		v[2] *= 2.0;
		v[3] *= 3.0;
		logtoall("volt=%f,%f,%f,%f\n", v[0], v[1], v[2], v[3]);

		*(u32*)(buf+0) = _v0v2_;
		*(u32*)(buf+4) = *(u32*)&v[3];
		give_data_into_peer_temp_stack(ele, _sock_, 0,0, buf, 8);

		*(u32*)(buf+0) = _v0v1_;
		*(u32*)(buf+4) = *(u32*)&v[1];
		give_data_into_peer_temp_stack(ele, _sock_, 0,0, buf, 8);

		//period
		sleep_us(1000*1000);
	}
}
