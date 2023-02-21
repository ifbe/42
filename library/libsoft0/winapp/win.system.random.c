#include<stdio.h>
#include<stdlib.h>
#include<time.h>




unsigned int random_read()
{
	return rand();
}




void freerandom()
{
}
void initrandom()
{
	srand((int)time(0));
}