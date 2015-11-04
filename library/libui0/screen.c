#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
void initwindow(QWORD,QWORD);
void killwindow();
QWORD currentresolution();




static unsigned long long resolution=(768<<16)+1024;
static unsigned long long data;




QWORD screendata()
{
	return data;
}
QWORD screenresolution()
{
	return currentresolution();
}








__attribute__((constructor(102))) void initscreen()
{
	data = (unsigned long long)malloc(0x400000);              //4M
	initwindow(data,resolution);
}
__attribute__((destructor)) void killscreen()
{
	killwindow();
	free((void*)data);
}