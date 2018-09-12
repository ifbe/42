#include "libuser.h"
int openreadclose(void*, int, void*, int);




void printhtmlnode(u8* buf, int len)
{
	int j;
	for(j=0;j<len;j++)
	{
		if('<' == buf[j])
		{
			if('!' == buf[j+1])continue;
			if('/' == buf[j+1])say("%.*s\n", 6, buf+j);
			else say("%.*s\n", 5, buf+j);
		}
	}
}
void printhtmlbody(u8* buf, int len)
{
	int j,k,t;
	int sp,flag;
	int stack[256][2];
	char* space = "                                                           ";

	sp = 0;
	for(j=0;j<len;j++)
	{
		if('<' != buf[j])continue;
		if('!' == buf[j+1])continue;

		t = 0;
		flag = 0;
		for(k=1;k<16;k++)
		{
			if(('/' == buf[j+k])&&('>' == buf[j+k+1]))flag = 1;
			if('>' == buf[j+k])break;
			if(' ' == buf[j+k])
			{
				for(t=k+1;t<len;t++)
				{
					if(('/' == buf[j+t])&&('>' == buf[j+t+1]))flag = 1;
					if('>' == buf[j+t])break;
				}
				break;
			}
		}
		//say("%.*s\n", k-1, buf+j+1);

		if(0 == ncmp(buf+j+1, "script", 6))
		{
			for(k=j+7;k<len;k++)
			{
				if('<' != buf[k])continue;
				if(0 == ncmp(buf+k, "</script", 8))
				{
					j = k+7;
					break;
				}
			}
			continue;
		}
		else if('/' == buf[j+1])
		{
			sp--;
			if(sp < 0)break;
		}
		else
		{
			if(sp>1)say("%.*s", (sp-1)*4, space);
			if(sp>0)say("└───");
			say("%.*s", k-1, buf+j+1);
			if(t)say(" %.*s", t-k-1, buf+j+k+1);
			say("\n");

			if(flag)continue;
			if(0 == ncmp(buf+j+1, "meta", 4))continue;
			if(0 == ncmp(buf+j+1, "br", 2))continue;
			stack[sp][0] = j+1;
			stack[sp][1] = k;
			sp++;
		}
	}
}




void scene_backup(u8* buf, int len)
{
	if(0 == len)
	{
		while(1)
		{
			if(buf[len] < 0x20)break;
			len++;
		}
	}
	say("%.*s\n", len, buf);
}
void scene_recover(u8* buf, int len)
{
	int j,k,ret;
	struct arena* win;
	u8* mem;
	u8 tmp[256];

	ret = 0;
	while(1)
	{
		if(buf[ret] < 0x20)break;
		tmp[ret] = buf[ret];
		ret++;
	}
	tmp[ret] = 0;
	say("name: %.*s\n", ret, tmp);

	win = arenacreate(_html_, 0);
	if(0 == win)return;
	say("win: %llx\n", win);

	mem = memorycreate(0x100000);
	if(0 == mem)return;
	say("mem: %llx\n", mem);

	win->buf = mem;
	ret = openreadclose(tmp, 0, mem, 0x100000);
	if(0 >= ret)return;
	say("read:%d\n", ret);

	printhtmlbody(mem, ret);
}
