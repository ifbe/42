#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int _dev_urandom=-1;




unsigned int getrandom()
{
	unsigned int value;
	int ret=read(_dev_urandom,(char*)&value,4);

	if(ret<4){printf("error reading:/dev/urandom:%x\n",value);}
	return value;
}




void createrandom()
{
	_dev_urandom=open("/dev/urandom",O_RDONLY);
	if(_dev_urandom==-1)
	{
		printf("error open:/dev/urandom:%x\n",_dev_urandom);
	}
}
void deleterandom()
{
	if(_dev_urandom != -1)close(_dev_urandom);
}
