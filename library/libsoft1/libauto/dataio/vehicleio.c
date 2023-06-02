#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
#define _sock_ hex32('s','o','c','k')
//
#define _time_ hex32('t','i','m','e')
#define _volt_ hex32('v','o','l','t')
#define _v0v2_ hex32('v','0','v','2')
#define _v0v1_ hex32('v','0','v','1')
int decstr2u64(u8* src, u64* dst);
int sleep_us(int);


char pintable[4][3] = {
	{13, 5, 6},	//left,front
	{26,12,16},	//left,back
	{22,17,27},	//right,front
	{ 4,15,14},	//right,back
};


#define ADS1X15_REG_POINTER_MASK (0x03)      ///< Point mask
#define ADS1X15_REG_POINTER_CONVERT (0x00)   ///< Conversion
#define ADS1X15_REG_POINTER_CONFIG (0x01)    ///< Configuration
#define ADS1X15_REG_POINTER_LOWTHRESH (0x02) ///< Low threshold
#define ADS1X15_REG_POINTER_HITHRESH (0x03)  ///< High threshold

#define ADS1X15_REG_CONFIG_OS_MASK (0x8000) ///< OS Mask
#define ADS1X15_REG_CONFIG_OS_SINGLE (0x8000) ///< Write: Set to start a single-conversion
#define ADS1X15_REG_CONFIG_OS_BUSY (0x0000) ///< Read: Bit = 0 when conversion is in progress
#define ADS1X15_REG_CONFIG_OS_NOTBUSY (0x8000) ///< Read: Bit = 1 when device is not performing a conversion

#define ADS1X15_REG_CONFIG_MUX_MASK (0x7000) ///< Mux Mask
#define ADS1X15_REG_CONFIG_MUX_DIFF_0_1 (0x0000) ///< Differential P = AIN0, N = AIN1 (default)
#define ADS1X15_REG_CONFIG_MUX_DIFF_0_3 (0x1000) ///< Differential P = AIN0, N = AIN3
#define ADS1X15_REG_CONFIG_MUX_DIFF_1_3 (0x2000) ///< Differential P = AIN1, N = AIN3
#define ADS1X15_REG_CONFIG_MUX_DIFF_2_3 (0x3000) ///< Differential P = AIN2, N = AIN3
#define ADS1X15_REG_CONFIG_MUX_SINGLE_0 (0x4000) ///< Single-ended AIN0
#define ADS1X15_REG_CONFIG_MUX_SINGLE_1 (0x5000) ///< Single-ended AIN1
#define ADS1X15_REG_CONFIG_MUX_SINGLE_2 (0x6000) ///< Single-ended AIN2
#define ADS1X15_REG_CONFIG_MUX_SINGLE_3 (0x7000) ///< Single-ended AIN3

#define ADS1X15_REG_CONFIG_PGA_MASK (0x0E00)   ///< PGA Mask
#define ADS1X15_REG_CONFIG_PGA_6_144V (0x0000) ///< +/-6.144V range = Gain 2/3
#define ADS1X15_REG_CONFIG_PGA_4_096V (0x0200) ///< +/-4.096V range = Gain 1
#define ADS1X15_REG_CONFIG_PGA_2_048V                                          \
  (0x0400) ///< +/-2.048V range = Gain 2 (default)
#define ADS1X15_REG_CONFIG_PGA_1_024V (0x0600) ///< +/-1.024V range = Gain 4
#define ADS1X15_REG_CONFIG_PGA_0_512V (0x0800) ///< +/-0.512V range = Gain 8
#define ADS1X15_REG_CONFIG_PGA_0_256V (0x0A00) ///< +/-0.256V range = Gain 16

#define ADS1X15_REG_CONFIG_MODE_MASK (0x0100)   ///< Mode Mask
#define ADS1X15_REG_CONFIG_MODE_CONTIN (0x0000) ///< Continuous conversion mode
#define ADS1X15_REG_CONFIG_MODE_SINGLE                                         \
  (0x0100) ///< Power-down single-shot mode (default)

#define ADS1X15_REG_CONFIG_RATE_MASK (0x00E0) ///< Data Rate Mask

#define ADS1X15_REG_CONFIG_CMODE_MASK (0x0010) ///< CMode Mask
#define ADS1X15_REG_CONFIG_CMODE_TRAD                                          \
  (0x0000) ///< Traditional comparator with hysteresis (default)
#define ADS1X15_REG_CONFIG_CMODE_WINDOW (0x0010) ///< Window comparator

#define ADS1X15_REG_CONFIG_CPOL_MASK (0x0008) ///< CPol Mask
#define ADS1X15_REG_CONFIG_CPOL_ACTVLOW (0x0000) ///< ALERT/RDY pin is low when active (default)
#define ADS1X15_REG_CONFIG_CPOL_ACTVHI (0x0008) ///< ALERT/RDY pin is high when active

#define ADS1X15_REG_CONFIG_CLAT_MASK (0x0004) ///< Determines if ALERT/RDY pin latches once asserted
#define ADS1X15_REG_CONFIG_CLAT_NONLAT (0x0000) ///< Non-latching comparator (default)
#define ADS1X15_REG_CONFIG_CLAT_LATCH (0x0004) ///< Latching comparator

#define ADS1X15_REG_CONFIG_CQUE_MASK (0x0003) ///< CQue Mask
#define ADS1X15_REG_CONFIG_CQUE_1CONV (0x0000) ///< Assert ALERT/RDY after one conversions
#define ADS1X15_REG_CONFIG_CQUE_2CONV (0x0001) ///< Assert ALERT/RDY after two conversions
#define ADS1X15_REG_CONFIG_CQUE_4CONV (0x0002) ///< Assert ALERT/RDY after four conversions
#define ADS1X15_REG_CONFIG_CQUE_NONE (0x0003) ///< Disable the comparator and put ALERT/RDY in high state (default)

#define RATE_ADS1015_128SPS (0x0000)  ///< 128 samples per second
#define RATE_ADS1015_250SPS (0x0020)  ///< 250 samples per second
#define RATE_ADS1015_490SPS (0x0040)  ///< 490 samples per second
#define RATE_ADS1015_920SPS (0x0060)  ///< 920 samples per second
#define RATE_ADS1015_1600SPS (0x0080) ///< 1600 samples per second (default)
#define RATE_ADS1015_2400SPS (0x00A0) ///< 2400 samples per second
#define RATE_ADS1015_3300SPS (0x00C0) ///< 3300 samples per second

#define RATE_ADS1115_8SPS (0x0000)   ///< 8 samples per second
#define RATE_ADS1115_16SPS (0x0020)  ///< 16 samples per second
#define RATE_ADS1115_32SPS (0x0040)  ///< 32 samples per second
#define RATE_ADS1115_64SPS (0x0060)  ///< 64 samples per second
#define RATE_ADS1115_128SPS (0x0080) ///< 128 samples per second (default)
#define RATE_ADS1115_250SPS (0x00A0) ///< 250 samples per second
#define RATE_ADS1115_475SPS (0x00C0) ///< 475 samples per second
#define RATE_ADS1115_860SPS (0x00E0) ///< 860 samples per second

typedef enum {
  GAIN_TWOTHIRDS = ADS1X15_REG_CONFIG_PGA_6_144V,
  GAIN_ONE = ADS1X15_REG_CONFIG_PGA_4_096V,
  GAIN_TWO = ADS1X15_REG_CONFIG_PGA_2_048V,
  GAIN_FOUR = ADS1X15_REG_CONFIG_PGA_1_024V,
  GAIN_EIGHT = ADS1X15_REG_CONFIG_PGA_0_512V,
  GAIN_SIXTEEN = ADS1X15_REG_CONFIG_PGA_0_256V
} adsGain_t;




void startADCReading(_obj* ele, u16 mux, int continuous) {
	// Start with default values
	u16 config =
	ADS1X15_REG_CONFIG_CQUE_1CONV |   // Set CQUE to any value other than
	// None so we can use it in RDY mode
	ADS1X15_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
	ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
	ADS1X15_REG_CONFIG_CMODE_TRAD;    // Traditional comparator (default val)

	if (continuous) {
	config |= ADS1X15_REG_CONFIG_MODE_CONTIN;
	} else {
	config |= ADS1X15_REG_CONFIG_MODE_SINGLE;
	}

	// Set PGA/voltage range
	config |= GAIN_TWOTHIRDS;

	// Set data rate
	config |= RATE_ADS1115_128SPS;

	// Set channels
	config |= mux;

	// Set 'start single-conversion' bit
	config |= ADS1X15_REG_CONFIG_OS_SINGLE;

	// Write config register to the ADC
	u8 buf[2];
	buf[0] = config >> 8;
	buf[1] = config & 0xff;
	give_data_into_peer(ele,_i2c_, 0,0, 0,0x480000|ADS1X15_REG_POINTER_CONFIG, buf,2);

	u16 h = 0x8000;
	buf[0] = h >> 8;
	buf[1] = h & 0xff;
	give_data_into_peer(ele,_i2c_, 0,0, 0,0x480000|ADS1X15_REG_POINTER_HITHRESH, buf,2);

	u16 l = 0x0000;
	buf[0] = l >> 8;
	buf[1] = l & 0xff;
	give_data_into_peer(ele,_i2c_, 0,0, 0,0x480000|ADS1X15_REG_POINTER_LOWTHRESH, buf,2);
}
int conversionComplete(_obj* ele) {
	u8 buf[2];
	take_data_from_peer(ele,_i2c_, 0,0, 0,0x480000|ADS1X15_REG_POINTER_CONFIG, buf,2);

	int val = (buf[0]<<8) | buf[1];
	return (val & 0x8000) != 0;
}
int getLastConversionResults(_obj* ele) {
	u8 buf[2];
	take_data_from_peer(ele,_i2c_, 0,0, 0,0x480000|ADS1X15_REG_POINTER_CONVERT, buf,2);

	int val = (buf[0]<<8) | buf[1];
	return val;
}
float readadc(_obj* ele, int chan)
{
	startADCReading(ele, (4+chan)<<12, 0);
	while (!conversionComplete(ele))sleep_us(1000);

	int val = getLastConversionResults(ele);
	float volt = (6.144 * val) / 32768;
	//say("chan=%d,val=%d,volt=%f\n",chan,val,volt);

	return volt;
}



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
		say("day=%d,hour=%d,minute=%d,second=%d\n", msg->_u8[3], msg->_u8[2], msg->_u8[1], msg->_u8[0]);
		break;
	case _v0v1_:
		say("v0v1=%f\n", msg->_f32);
		break;
	case _v0v2_:
		say("v0v2=%f\n", msg->_f32);
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
	case 'w':
	case 0x415b1b:
		say("front\n");
		ch[1] = 'w';
		break;
	case 's':
	case 0x425b1b:
		say("back\n");
		ch[1] = 's';
		break;
	case 'a':
	case 0x445b1b:
		say("left\n");
		ch[1] = 'a';
		break;
	case 'd':
	case 0x435b1b:
		say("right\n");
		ch[1] = 'd';
		break;
	default:
		say("other\n");
		ch[1] = 0;
	}

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
	//say("@vehicleclient_giveby:len=%x,foot=%.4s\n",len,&stack[sp-1].foottype);
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
	say("@vehicleclient_create\n");

	struct perobj* perobj = (void*)ele->priv_256b;

	//initgpio
	//initi2c
	return 1;
}
int vehicleclient_delete(_obj* ele)
{
	return 0;
}


void vehicleserver_sock(_obj* art,void* foot, void* buf,int len)
{
	int k;
	u8 u[4][3];

	u32* in = buf;
	say("vehicleserver_sock:%x,%x\n",in[0],in[1]);
	switch(in[1]){
	case 'w':
		for(k=0;k<4;k++){
			u[k][0] = 1;
			u[k][1] = 0;
			u[k][2] = 1;
		}
		printmemory(u,12);
		give_data_into_peer_temp_stack(art, _gpio_, pintable,_volt_, u,12);
		sleep_us(200*1000);
		break;
	case 's':
		for(k=0;k<4;k++){
			u[k][0] = 1;
			u[k][1] = 1;
			u[k][2] = 0;
		}
		printmemory(u,12);
		give_data_into_peer_temp_stack(art, _gpio_, pintable,_volt_, u,12);
		sleep_us(200*1000);
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
		give_data_into_peer_temp_stack(art, _gpio_, pintable,_volt_, u,12);
		sleep_us(200*1000);
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
		give_data_into_peer_temp_stack(art, _gpio_, pintable,_volt_, u,12);
		sleep_us(200*1000);
		break;
	default:
		for(k=0;k<4;k++)u[k][0] = 0;
		printmemory(u,12);
		give_data_into_peer_temp_stack(art, _gpio_, pintable,_volt_, u,12);
	}
}


int vehicleserver_takeby(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vehicleserver_giveby(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	struct perobj* perobj = (void*)art->priv_256b;
	//say("@vehicleserver_giveby:len=%x,foot=%.4s\n",len,&stack[sp-1].foottype);
	switch(stack[sp-1].foottype){
	case _sock_:
		vehicleserver_sock(art,foot, buf,len);
	}
	return 0;
}
int vehicleserver_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vehicleserver_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@vehicleserver_attach\n");
	int j;
	u8 f[12];
	for(j=0;j<12;j++)f[j] = 1;

	switch(self->foottype){
	case _gpio_:
		give_data_into_peer_temp_stack(self->pchip, _gpio_, pintable,_func_, f, 12);
		break;
	}
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
	say("@vehicleserver_create\n");

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
		time = dateread();
		say("time=%llx\n", time);

		*(u32*)(buf+0) = _time_;
		*(u32*)(buf+4) = time&0xffffffff;
		give_data_into_peer_temp_stack(ele, _sock_, 0,0, buf, 8);

		v[0] = readadc(ele, 0) * 3.2;
		v[1] = readadc(ele, 1) * 2.0;
		v[2] = readadc(ele, 2);
		v[3] = readadc(ele, 3) * 2.0;
		say("%f,%f,%f,%f\n", v[0], v[1], v[2], v[3]);

		*(u32*)(buf+0) = _v0v2_;
		*(u32*)(buf+4) = *(u32*)&v[0];
		give_data_into_peer_temp_stack(ele, _sock_, 0,0, buf, 8);

		*(u32*)(buf+0) = _v0v1_;
		*(u32*)(buf+4) = *(u32*)&v[1];
		give_data_into_peer_temp_stack(ele, _sock_, 0,0, buf, 8);

		sleep_us(1000*1000);
	}
}
