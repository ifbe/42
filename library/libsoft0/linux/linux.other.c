#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int _dev_random=-1;




unsigned int getrandom()
{
	unsigned int value;
	int ret=read(_dev_random,(char*)&value,4);

	if(ret<4){printf("error reading:/dev/random:%x\n",value);}
	return value;
}
void initother()
{
	_dev_random=open("/dev/random",O_RDONLY);
	if(_dev_random==-1)
	{
		printf("error open:/dev/random:%x\n",_dev_random);
	}
}
void killother()
{
	if(_dev_random != -1)close(_dev_random);
}
