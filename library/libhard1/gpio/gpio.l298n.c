#include "libsoft.h"
#define _chip_ hex32('c','h','i','p')
#define _pin_value_ hex32('p','v', 0, 0)
#define _pin_mode_value_ hex32('p','m','v', 0)
int sleep_us(int);




/*
3v3             ----________----5v
g02-sda         ----________----5v
g03-scl         ----________----0v
g04-motor-lf-n  ----________----g14-motor-rf-p
0v              ----________----g15-motor-rf-n
g17-motor-rf-en ----________----g18-i2s-clk
g27-motor-lf-p  ----________----0v
g22-motor-lf-en ----________----g23-sensehat-imu-int1
3v3             ----____r___----g24-sensehat-imu-int2
g10-spi0-mosi   ----____p___----0v
g09-spi0-miso   ----____i___----g25-sensehat-atmel-chip-reset
g11-spi0-sclk   ----____x___--- g08-sensehat-atmel-chip-select(spi0-ce0)
0v              ----________----g07-spi0-ce1
g00-used-3.3v   ----________----g01-motor-rn-n
g05-motor-ln-n  ----________----0v
g06-motor-ln-p  ----________----g12-motor-rn-en
g13-motor-ln-en ----________----0v
g19-i2s-fs      ----________----g16-used-sdmode
g26-motor-rn-p  ----________----g20-i2s-din
0v              ----________----g21-i2s-dout
*/
static char pintable[4][3] = {
	{22, 4,27},	//left,front
	{13, 5, 6},	//left,back
	{17,15,14},	//right,front
	{12, 1,26},	//right,back
};
static char pin_value[12][2] = {
	{22, 0},
	{ 4, 0},
	{27, 0},
	{13, 0},
	{ 5, 0},
	{ 6, 0},
	{17, 0},
	{15, 0},
	{14, 0},
	{12, 0},
	{ 1, 0},
	{26, 0},
};
static char pin_mode_value[12][3] = {
	{22, 'o', 0},	//(left, front) en,n,p
	{ 4, 'o', 0},
	{27, 'o', 0},
	{13, 'o', 0},	//(left, near)
	{ 5, 'o', 0},
	{ 6, 'o', 0},
	{17, 'o', 0},	//(right, front)
	{15, 'o', 0},
	{14, 'o', 0},
	{12, 'o', 0},	//(right, near)
	{ 1, 'o', 0},
	{26, 'o', 0},
};


//lf ln rf rn
signed char action_stop[][4] = {
	{0, 0, 0, 0}
};
signed char action_test[][4] = {
	{ 1, 0, 0, 0},	//1
	{-1, 0, 0, 0},	//2
	{ 0, 1, 0, 0},	//3
	{ 0,-1, 0, 0},	//4
	{ 0, 0, 1, 0},	//5
	{ 0, 0,-1, 0},	//6
	{ 0, 0, 0, 1},	//7
	{ 0, 0, 0,-1},	//8
	{}
};
signed char action_simple[][4] = {
	{ 1, 1, 1, 1},	//w
	{-1,-1,-1,-1},	//x
	{ 0, 0, 0, 0},	//a
	{ 0, 0, 0, 0},	//d
	{ 0, 0, 0, 0},	//e
	{ 0, 0, 0, 0},	//z
	{ 0, 0, 0, 0},	//q
	{ 0, 0, 0,-0},	//c
	{-1,-1, 1, 1},	//j
	{ 1, 1,-1,-1},	//k
	{}
};
signed char action_omni_o[][4] = {
	{ 1, 1, 1, 1},	//w
	{-1,-1,-1,-1},	//x
	{-1, 1, 1,-1},	//a
	{ 1,-1,-1, 1},	//d
	{ 1, 0, 0, 1},	//e
	{-1, 0, 0,-1},	//z
	{ 0, 1, 1, 0},	//q
	{ 0,-1,-1, 0},	//c
	{-1,-1, 1, 1},	//j
	{ 1, 1,-1,-1},	//k
	{}
};
signed char action_omni_x[][4] = {
	{ 1, 1, 1, 1},	//w
	{-1,-1,-1,-1},	//x
	{ 1,-1,-1, 1},	//a
	{-1, 1, 1,-1},	//d
	{ 0, 1, 1, 0},	//e
	{ 0,-1,-1, 0},	//z
	{ 1, 0, 0, 1},	//q
	{-1, 0, 0,-1},	//c
	{ 1, 1, 0, 0},	//j
	{ 0, 0, 1, 1},	//k
	{}
};
signed char action_mecanam_o[][4] = {
	{}
};
signed char action_mecanam_x[][4] = {
	{ 1, 1, 1, 1},	//w
	{-1,-1,-1,-1},	//x
	{-1, 1,-1, 1},	//a
	{ 1,-1, 1,-1},	//d
	{ 1, 0, 1, 0},	//e
	{-1, 0,-1, 0},	//z
	{ 0, 1, 0, 1},	//q
	{ 0,-1, 0,-1},	//c
	{-1,-1, 1, 1},	//j
	{ 1, 1,-1,-1},	//k
	{}
};
struct privdata{
	signed char (*action)[4];
};


//careful: some compiler must use "signed" char
void action2pinval(signed char* act, char (*pv)[2])
{
	int k;
	int en,val;
	for(k=0;k<4;k++){
		en = act[k]&1;
		pin_value[k*3+0][1] = en;

		if(en){
			val = (act[k]>0) ? 1 : 0;
			logtoall("k=%d,act=%d,val=%d\n",k,act[k],val);
			pin_value[k*3+1][1] =!val;
			pin_value[k*3+2][1] = val;
		}
	}
}


int l298n_take(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	logtoall("@l298n_take\n");

	return 0;
}
int l298n_give(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	int k;
	u32 in = *(u8*)buf;
	struct privdata* priv = (void*)obj->priv_256b;
	logtoall("vehicleserver_sock:%x\n",in);
	switch(in){
	case '1':
		action2pinval(action_test[0], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case '2':
		action2pinval(action_test[1], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case '3':
		action2pinval(action_test[2], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case '4':
		action2pinval(action_test[3], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case '5':
		action2pinval(action_test[4], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case '6':
		action2pinval(action_test[5], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case '7':
		action2pinval(action_test[6], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case '8':
		action2pinval(action_test[7], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 'w':
		action2pinval(priv->action[0], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 's':
		action2pinval(priv->action[1], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 'a':
		action2pinval(priv->action[2], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 'd':
		action2pinval(priv->action[3], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 'e':
		action2pinval(priv->action[4], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 'z':
		action2pinval(priv->action[5], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 'q':
		action2pinval(priv->action[6], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 'c':
		action2pinval(priv->action[7], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(300*1000);
		break;
	case 'j':
		action2pinval(priv->action[8], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(50*1000);
		break;
	case 'k':
		action2pinval(priv->action[9], pin_value);
		writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
		sleep_us(50*1000);
		break;
	}

	action2pinval(action_stop[0], pin_value);
	writing_data_into_peer(obj, _chip_, 0, _pin_value_, pin_value,12);
	return 0;
}
int l298n_attach(struct halfrel* self, struct halfrel* peer)
{
	int j;
	switch(self->foottype){
	case _chip_:
		for(j=0;j<12;j++)pin_mode_value[j][1] = 'o';
		writing_data_into_peer(self->pchip, _chip_, 0, _pin_mode_value_, pin_mode_value, 12);
		break;
	}
	return 0;
}
int l298n_detach(struct halfrel* self, struct halfrel* peer)
{
	int j;
	switch(self->foottype){
	case _chip_:
		for(j=0;j<12;j++)pin_mode_value[j][1] = 'i';
		writing_data_into_peer(self->pchip, _chip_, 0, _pin_mode_value_, pin_mode_value, 12);
		break;
	}
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
	logtoall("@l298n_create\n");
	struct privdata* priv = (void*)obj->priv_256b;

	int j;
	int type = 1;
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "wheel:", 6)){
			if(0 == ncmp(argv[j]+6, "none", 4))type = 0;
			else if(0 == ncmp(argv[j]+6, "simple", 6))type = 1;
			else if(0 == ncmp(argv[j]+6, "meca_o", 6))type = 2;
			else if(0 == ncmp(argv[j]+6, "meca_x", 6))type = 3;
			else if(0 == ncmp(argv[j]+6, "omni_o", 6))type = 4;
			else if(0 == ncmp(argv[j]+6, "omni_x", 6))type = 5;
			logtoall("wheel=%d\n", type);
		}
	}

	switch(type){
	case 0:
		priv->action = 0;
	case 1:
		priv->action = action_simple;
		break;
	case 2:
		priv->action = action_mecanam_o;
		break;
	case 3:
		priv->action = action_mecanam_x;
		break;
	case 4:
		priv->action = action_omni_o;
		break;
	case 5:
		priv->action = action_omni_x;
		break;
	}
	return 1;
}
int l298n_delete(_obj* ele)
{
	return 0;
}
