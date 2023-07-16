#include "libsoft.h"
#define _volt_ hex32('v','o','l','t')
int sleep_us(int);




char pintable[4][3] = {
	{13, 5, 6},	//left,front
	{26,12,16},	//left,back
	{22,17,27},	//right,front
	{ 4,15,14},	//right,back
};




int l298n_take(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	say("@l298n_take\n");

    return 0;
}
int l298n_give(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	int k;
	u8 u[4][3];

	u32 in = *(u32*)buf;
	say("vehicleserver_sock:%x\n",in);
	switch(in){
	case 'w':
		for(k=0;k<4;k++){
			u[k][0] = 1;
			u[k][1] = 0;
			u[k][2] = 1;
		}
		printmemory(u,12);
		writing_data_into_peer(obj, _gpio_, (p64)pintable,_volt_, u,12);
		sleep_us(100*1000);
		break;
	case 's':
		for(k=0;k<4;k++){
			u[k][0] = 1;
			u[k][1] = 1;
			u[k][2] = 0;
		}
		printmemory(u,12);
		writing_data_into_peer(obj, _gpio_, (p64)pintable,_volt_, u,12);
		sleep_us(100*1000);
		break;
	case 'a':
		for(k=0;k<2;k++){
			u[k][0] = 1;
			u[k][1] = 1;
			u[k][2] = 0;
		}
		for(k=2;k<4;k++){
			u[k][0] = 1;
			u[k][1] = 0;
			u[k][2] = 1;
		}
		printmemory(u,12);
		writing_data_into_peer(obj, _gpio_, (p64)pintable,_volt_, u,12);
		sleep_us(100*1000);
		break;
	case 'd':
		for(k=0;k<2;k++){
			u[k][0] = 1;
			u[k][1] = 0;
			u[k][2] = 1;
		}
		for(k=2;k<4;k++){
			u[k][0] = 1;
			u[k][1] = 1;
			u[k][2] = 0;
		}
		printmemory(u,12);
		writing_data_into_peer(obj, _gpio_, (p64)pintable,_volt_, u,12);
		sleep_us(100*1000);
		break;
	}

	for(k=0;k<4;k++)u[k][0] = 0;
	printmemory(u,12);
	writing_data_into_peer(obj, _gpio_, (p64)pintable,_volt_, u,12);
    return 0;
}
int l298n_attach(struct halfrel* self, struct halfrel* peer)
{
	int j;
	u8 f[12];
	for(j=0;j<12;j++)f[j] = 1;

	switch(self->foottype){
	case _gpio_:
		writing_data_into_peer(self->pchip, _gpio_, (p64)pintable,_func_, f, 12);
		break;
	}
    return 0;
}
int l298n_detach(struct halfrel* self, struct halfrel* peer)
{
    return 0;
}




int l298n_read(_obj* obj,void* foot, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int l298n_write(_obj* obj,void* foot,p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int l298n_create(_obj* obj, void* arg, int argc, u8** argv)
{
	say("@l298n_create\n");
	return 1;
}
int l298n_delete(_obj* ele)
{
	return 0;
}