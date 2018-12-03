#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "libboot.h"
#define _gpio_ hex32('g','p','i','o')
#define _pwm_  hex32('p','w','m',0)
#define _car_  hex32('c','a','r',0)




void carmove(int L, int R)
{
	digitalWrite( 3, HIGH);
	digitalWrite(21, L);
	digitalWrite(22, !L);
	digitalWrite(23, L);
	digitalWrite(24, !L);
	digitalWrite(25, HIGH);

	digitalWrite( 5, HIGH);
	digitalWrite( 6, R);
	digitalWrite(26, !R);
	digitalWrite(27, R);
	digitalWrite(28, !R);
	digitalWrite(29, HIGH);
}
void carstart()
{
	wiringPiSetup();

	pinMode( 3, OUTPUT);
	pinMode(21, OUTPUT);
	pinMode(22, OUTPUT);
	pinMode(23, OUTPUT);
	pinMode(24, OUTPUT);
	pinMode(25, OUTPUT);

	pinMode( 5, OUTPUT);
	pinMode( 6, OUTPUT);
	pinMode(26, OUTPUT);
	pinMode(27, OUTPUT);
	pinMode(28, OUTPUT);
	pinMode(29, OUTPUT);
}
void carstop()
{
	digitalWrite( 3, LOW);
	digitalWrite(21, LOW);
	digitalWrite(22, LOW);
	digitalWrite(23, LOW);
	digitalWrite(24, LOW);
	digitalWrite(25, LOW);

	digitalWrite( 5, LOW);
	digitalWrite( 6, LOW);
	digitalWrite(26, LOW);
	digitalWrite(27, LOW);
	digitalWrite(28, LOW);
	digitalWrite(29, LOW);

	pinMode( 3, INPUT);
	pinMode(21, INPUT);
	pinMode(22, INPUT);
	pinMode(23, INPUT);
	pinMode(24, INPUT);
	pinMode(25, INPUT);

	pinMode( 5, INPUT);
	pinMode( 6, INPUT);
	pinMode(26, INPUT);
	pinMode(27, INPUT);
	pinMode(28, INPUT);
	pinMode(29, INPUT);
}




int boardread(int type, int addr, u8* buf, int len)
{
	switch(type)
	{
		case _gpio_:
		{
			return digitalRead(addr);
			break;
		}
		default:printf("%x,%x,%x,%x\n", type, addr, buf, len);
	}
	return 0;
}
int boardwrite(int type, int addr, u8* buf, int len)
{
	switch(type)
	{
		case _gpio_:
		{
			//_gpio_, 29, "o,1"
			switch(buf[0])
			{
				case 'i':pinMode(addr, INPUT);break;
				case 'o':pinMode(addr, OUTPUT);break;
				case '1':digitalWrite(addr, HIGH);break;
				case '0':digitalWrite(addr, LOW);break;
			}
			break;
		}
		case _pwm_:
		{
			//_pwm_, 18, "1ms/2ms"
			break;
		}
		case _car_:
		{
			//_car_, 0, "start, farward"
			switch(buf[0])
			{
				case 'l':carmove(1, 0);break;
				case 'r':carmove(0, 1);break;
				case 'n':carmove(0, 0);break;
				case 'f':carmove(1, 1);break;
				case '+':carstart();break;
				case '-':carstop();break;
			}
			break;
		}
		default:printf("%x,%x,%x,%x\n", type, addr, buf, len);
	}
	return 0;
}
