#include "libsoft.h"
int sleep_us(int);
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
#define ADS1X15_REG_CONFIG_PGA_2_048V (0x0400) ///< +/-2.048V range = Gain 2 (default)
#define ADS1X15_REG_CONFIG_PGA_1_024V (0x0600) ///< +/-1.024V range = Gain 4
#define ADS1X15_REG_CONFIG_PGA_0_512V (0x0800) ///< +/-0.512V range = Gain 8
#define ADS1X15_REG_CONFIG_PGA_0_256V (0x0A00) ///< +/-0.256V range = Gain 16

#define ADS1X15_REG_CONFIG_MODE_MASK (0x0100)   ///< Mode Mask
#define ADS1X15_REG_CONFIG_MODE_CONTIN (0x0000) ///< Continuous conversion mode
#define ADS1X15_REG_CONFIG_MODE_SINGLE (0x0100) ///< Power-down single-shot mode (default)

#define ADS1X15_REG_CONFIG_RATE_MASK (0x00E0) ///< Data Rate Mask

#define ADS1X15_REG_CONFIG_CMODE_MASK (0x0010) ///< CMode Mask
#define ADS1X15_REG_CONFIG_CMODE_TRAD (0x0000) ///< Traditional comparator with hysteresis (default)
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




int ads1115_take(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	//say("@ads1115_take\n");
	float* v = buf;
	v[0] = readadc(obj, 0);
	v[1] = readadc(obj, 1);
	v[2] = readadc(obj, 2);
	v[3] = readadc(obj, 3);
	say("@ads1115_take: %f,%f,%f,%f\n", v[0], v[1], v[2], v[3]);

	return 0;
}
int ads1115_give(_obj* obj,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int ads1115_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int ads1115_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int ads1115_read(_obj* obj,void* foot, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int ads1115_write(_obj* obj,void* foot,p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int ads1115_create(_obj* obj, void* arg, int argc, u8** argv)
{
	say("@ads1115_create\n");
	return 1;
}
int ads1115_delete(_obj* ele)
{
	return 0;
}