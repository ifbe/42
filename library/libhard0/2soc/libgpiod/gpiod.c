#include <stdio.h>
#include <unistd.h>
#include <gpiod.h>
#include "libhard.h"
#define _pin_mode_value_ hex32('p','m','v', 0)
#define _volt_ hex32('v','o','l','t')


struct privdata{
	struct gpiod_chip* chip;
	struct gpiod_line** pin;
};

void setpin_input(struct privdata* priv, int id)
{
	struct gpiod_line* gpioline = gpiod_chip_get_line(priv->chip, id);
	if (gpioline == NULL)return;
	say("gpiod_chip_get_line ok\n");

	int ret = gpiod_line_request_input(gpioline, "gpioin");
	if(ret != 0)return;
	say("gpiod_line_request_input: ret=%d\n", ret);

	priv->pin[id] = gpioline;
}
void setpin_output(struct privdata* priv, int id, int val)
{
	struct gpiod_line* gpioline = gpiod_chip_get_line(priv->chip, id);
	if (gpioline == NULL)return;
	say("gpiod_chip_get_line ok\n");

	int ret = gpiod_line_request_output(gpioline, "gpioout", val);
	if (ret != 0)return;
	say("gpiod_line_request_output ok\n");

	if (gpiod_line_is_used(gpioline)){
		say("gpiod_line_is_used\n");
		//goto errorline;
	}

	priv->pin[id] = gpioline;
}




int getvalue(struct privdata* priv, int id)
{
	if(0 == priv)return -1;
	if(0 == priv->pin[id])return -2;

	int val = gpiod_line_get_value(priv->pin[id]);
	say("gpiod_line_get_value: val=%d\n", val);
	return val;
}
int setvalue(struct privdata* priv, int id, int val)
{
	if(0 == priv)return -1;
	if(0 == priv->pin[id])return -2;

	int ret = gpiod_line_set_value(priv->pin[id], val);
	say("gpiod_line_set_value: val=%d, ret=%d\n", val, ret);
	return 0;
}




int gpio_takeby(struct halfrel* st,int sp, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
int gpio_giveby(struct halfrel* st,int sp, p64 arg,int cmd, void* buf,int len)
{
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
	say("@gpioread:%p,%x,%p,%x\n", arg, cmd, buf, len);
	/*
		input: arg=u8list{pin}, buf=list{val}
	*/
	int j;
	for(j=0;j<len;j++)buf[j] = getvalue(priv, j);
	return 0;
}
int gpio_write(_obj* obj,void* foot, u8* arg,int cmd, u8* buf,int len)
{
	say("@gpiowrite:%p,%x,%p,%x\n", arg, cmd, buf, len);
	struct privdata* priv = (void*)obj->priv_256b;

	int j;
	int pin,mode;
	switch(cmd){
	case _volt_:
	/*
		input: arg=u8list{pin}, buf=list{val}
	*/
		for(j=0;j<len;j++){
			pin = arg[j];
			setvalue(priv, pin, buf[j]);
		}
		break;
	case _pin_mode_value_:
	/*
		input: arg=u8list{pin,mode,value}, buf=list{val}
	*/
		for(j=0;j<len;j++){
			pin = arg[j*3+0];
			mode = arg[j*3+1];
			switch(mode){
			case 'i':
				setpin_input(priv, pin);
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
			say("gpiod_line_release: pin=%d,line=%p\n", j, priv->pin[j]);
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

	//chip open
	struct gpiod_chip* gpiochip = gpiod_chip_open(path);
	if (gpiochip == NULL)goto errorchip;
	say("gpiod_chip_open ok\n");

	//my device
	if(0 == obj)obj = device_alloc(_dev_mem);
	struct privdata* priv = (void*)obj->priv_256b;
	priv->chip = gpiochip;
	priv->pin = memoryalloc(0x1000, 0);

	return 0;
}
