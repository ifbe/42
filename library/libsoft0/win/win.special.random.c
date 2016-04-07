#include<stdio.h>
#include<stdlib.h>
#include<time.h>




unsigned int getrandom()
{
	return rand();
}
void initrandom()
{
	srand((int)time(0));
}
void killrandom()
{
}
