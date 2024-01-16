#include <stdio.h>
#include <unistd.h>
#include <gpiod.h>
#include "libhard.h"
#define _volt_ hex32('v','o','l','t')
#define _pin_value_ hex32('p','v', 0, 0)
#define _pin_mode_value_ hex32('p','m','v', 0)
void* device_alloc_prep(u64,u64,u64,u64);


struct privdata{
	struct gpiod_chip* chip;
	struct gpiod_line** pin;
};

void setpin_input(struct privdata* priv, int id)
{
	struct gpiod_line* gpioline = gpiod_chip_get_line(priv->chip, id);
	if (gpioline == NULL)return;
	logtoall("gpiod_chip_get_line %d ok\n", id);

	int ret = gpiod_line_request_input(gpioline, "gpioin");
	if(ret != 0)return;
	logtoall("gpiod_line_request_input %d ok\n", id);

	priv->pin[id] = gpioline;
}
void setpin_output(struct privdata* priv, int id, int val)
{
	struct gpiod_line* gpioline = gpiod_chip_get_line(priv->chip, id);
	if (gpioline == NULL)return;
	logtoall("gpiod_chip_get_line %d ok\n", id);

	int ret = gpiod_line_request_output(gpioline, "gpioout", val);
	if (ret != 0)return;
	logtoall("gpiod_line_request_output %d ok\n", id);

	if (gpiod_line_is_used(gpioline)){
		logtoall("gpiod_line_is_used %d\n", id);
		//goto errorline;
	}

	priv->pin[id] = gpioline;
}




int getvalue(struct privdata* priv, int id)
{
	if(0 == priv)return -1;
	if(0 == priv->pin[id])return -2;

	int val = gpiod_line_get_value(priv->pin[id]);
	logtoall("gpiod_line_get_value: gpio=%d, val=%d\n", id, val);
	return val;
}
int setvalue(struct privdata* priv, int id, int val)
{
	if(0 == priv)return -1;
	if(0 == priv->pin[id])return -2;

	int ret = gpiod_line_set_value(priv->pin[id], val);
	logtoall("gpiod_line_set_value: gpio=%d, val=%d, ret=%d\n", id, val, ret);
	return 0;
}




int gpio_takeby(struct halfrel* st,int sp, p64 arg,int cmd, void* buf,int len)
{
	logtoall("@gpio_takeby:%p,%x,%p,%x\n", arg, cmd, buf, len);
	return 0;
}
int gpio_giveby(struct halfrel* st,int sp, p64 arg,int cmd, void* buf,int len)
{
	logtoall("@gpio_giveby:%p,%x,%p,%x\n", arg, cmd, buf, len);
	return 0;
}
int gpio_detach(struct halfrel* st, struct halfrel* peer)
{
	return 0;
}
int gpio_attach(struct halfrel* st, struct halfrel* peer)
{
	return 0;
}
int gpio_read(_obj* obj,void* foot, p64 arg,int cmd, u8* buf,int len)
{
	logtoall("@gpioread:%p,%x,%p,%x\n", arg, cmd, buf, len);
	struct privdata* priv = (void*)obj->priv_256b;

	int j;
	switch(cmd){
	case _pin_value_:
	/*
		input: buf=u8list{pin,value}
	*/
		for(j=0;j<len;j++){
			buf[j*2+1] = getvalue(priv, buf[j*2+0]);
		}
		break;
	}
	return 0;
}
int gpio_write(_obj* obj,void* foot, u8* arg,int cmd, u8* buf,int len)
{
	logtoall("@gpiowrite:%p,%x,%p,%x\n", arg, cmd, buf, len);
	struct privdata* priv = (void*)obj->priv_256b;

	int j;
	int pin,mode,value;
	switch(cmd){
	case _pin_value_:
	/*
		input: buf=u8list{pin,value}
	*/
		for(j=0;j<len;j++){
			pin = buf[j*2+0];
			value = buf[j*2+1];
			logtoall("pin=%d,value=%d\n", pin, value);
			setvalue(priv, pin, value);
		}
		break;
	case _pin_mode_value_:
	/*
		input: buf=u8list{pin,mode,value}
	*/
		for(j=0;j<len;j++){
			pin = buf[j*3+0];
			mode = buf[j*3+1];
			switch(mode){
			case 'i':
				setpin_input(priv, pin);
				break;
			case 'o':
				setpin_output(priv, pin, 0);
				break;
			}
		}
		break;
	}
	return 0;
}
int gpio_delete(_obj* obj, void* arg)
{
	int j;
	struct privdata* priv = (void*)obj->priv_256b;
	if(priv->pin){
		for(j=0;j<100;j++){
			if(0 == priv->pin[j])continue;
			logtoall("gpiod_line_release: pin=%d,line=%p\n", j, priv->pin[j]);
			gpiod_line_release(priv->pin[j]);
			priv->pin[j] = 0;
		}
	}
	if(priv->chip){
		gpiod_chip_close(priv->chip);
		priv->chip = 0;
	}
	return 0;
}
int gpio_create(_obj* obj, void* arg, int argc, void** argv)
{
	//parse arg
	char* path = arg;
	logtoall("path=%s\n", path);

	//chip open
	struct gpiod_chip* gpiochip = gpiod_chip_open(path);
	if (gpiochip == NULL){
		logtoall("gpiod_chip_open failed: %s\n", path);
	}

	//my device
	if(0 == obj)obj = device_alloc_prep(0, _gpio_, 0, 0);
	struct privdata* priv = (void*)obj->priv_256b;
	priv->chip = gpiochip;
	priv->pin = memoryalloc(0x1000, 0);

	return 0;
}
