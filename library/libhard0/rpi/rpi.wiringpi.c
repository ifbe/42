#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "libboot.h"
#define _gpio_ hex32('g','p','i','o')
#define _pwm_  hex32('p','w','m',0)




int gpioread(int type, int addr, u8* buf, int len)
{
	switch(type){
	case _gpio_:{
		return digitalRead(addr);
		break;
	}
	default:{
		printf("%x,%x,%x,%x\n", type, addr, buf, len);
	}//default
	}//switch
	return 0;
}
int gpiowrite(int type, int addr, u8* buf, int len)
{
	//say("@gpiowrite: %.4s, %d, %d\n", &type, addr, len);
	switch(type){
	case _gpio_:{
		digitalWrite(addr, len);
		break;
	}
	case _pwm_:{
		//_pwm_, 18, "1ms/2ms"
		break;
	}
	default:{
		printf("%x,%x,%x,%x\n", type, addr, buf, len);
	}//default
	}//switch
	return 0;
}




int gpiostop(int pin)
{
	pinMode(pin, INPUT);
	return 0;
}
int gpiostart(int name, int mode)
{
	int pin = -1;
	switch(name)
	{
		case hex32('l','f','e',0):pin =  5;break;
		case hex32('l','f','p',0):pin = 21;break;
		case hex32('l','f','n',0):pin = 22;break;
		case hex32('l','n','p',0):pin = 23;break;
		case hex32('l','n','n',0):pin = 24;break;
		case hex32('l','n','e',0):pin = 29;break;
		case hex32('r','f','e',0):pin = 25;break;
		case hex32('r','f','p',0):pin =  6;break;
		case hex32('r','f','n',0):pin = 26;break;
		case hex32('r','n','p',0):pin = 27;break;
		case hex32('r','n','n',0):pin = 28;break;
		case hex32('r','n','e',0):pin =  3;break;

		case hex32('e','n', 0, 0):pin = 21;break;
		case hex32('x','d', 0, 0):pin = 22;break;
		case hex32('x','s', 0, 0):pin = 23;break;
		case hex32('y','d', 0, 0):pin = 24;break;
		case hex32('y','s', 0, 0):pin = 25;break;
		case hex32('z','d', 0, 0):pin = 26;break;
		case hex32('z','s', 0, 0):pin = 27;break;
		case hex32('w','d', 0, 0):pin = 28;break;
		case hex32('w','s', 0, 0):pin = 29;break;
		default:return -1;
	}
	say("@gpiostart: name=%x,mode=%x,pin=%d\n", name, mode, pin);

	switch(mode)
	{
		case 'i':
		{
			pinMode(pin, INPUT);
			break;
		}
		case 'o':
		{
			pinMode(pin, OUTPUT);
			break;
		}
		case hex32('p','w','m','o'):
		{
			pinMode(pin, PWM_OUTPUT);
			break;
		}
	}
	return pin;
}




int initgpio()
{
	return 0;
}
int initgpio()
{
	return wiringPiSetup();
}
