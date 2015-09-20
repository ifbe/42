#include<stdio.h>
#include<stdlib.h>




static unsigned long long screen;




__attribute__((constructor)) void initscreen()
{
	screen = (unsigned long long)malloc(0x400000);              //4M
	initwindow(screen);
}
__attribute__((destructor)) void killscreen()
{
	killwindow();
	free((void*)screen);
}




unsigned long long whereisscreen()
{
	return screen;
}
unsigned long long howisscreen()
{
	return (768<<16) + 1024;
}