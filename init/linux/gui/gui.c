#include<stdio.h>
#include<stdlib.h>




static unsigned char* screen;




void initscreen()
{
        screen = (unsigned char*)malloc(0x400000);              //4M
	initwindow(screen);
}
void killscreen()
{
	killwindow();
	free(screen);
}




unsigned long long whereisscreen()
{
        return (unsigned long long)screen;
}
