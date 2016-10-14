#include<windows.h>
static int mode = 0;




int terminal_read(char* p, int max)
{
	int i;
	char* ret;

	if(mode == 0)
	{
		while(1)
		{
			ret=fgets(p, max, stdin);
			//say("uievent.ret=%x\n",ret);

			if(ret == NULL)return 0;
			if(p[0] != 0)return 1;
		}
	}
	else if(mode == 1)
	{
		while(1)
		{
			i = getchar();
			printf("%x\n",i);
		}
		return 0;
	}
}




void terminal_passthrough()
{
}
void terminal_normalize()
{
}
