#include "actor.h"
//
int netmgr_write(void*);
//
int sound_start();
int vision_start();
//
int i2c_list();
int i2c_choose(void*);
//
int arenastart(u64, int);
int arenastop();
int actorlist(void*);
int actorchoose(void*);
//
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2type(u8* buf,int max,u64* type,u8** name);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void eventwrite(u64,u64,u64,u64);




static u8* input = 0;
static u8* output = 0;




void term_read(u8* buf)
{
	//say("here\n");
	if(buf == 0)goto empty;
	if( (buf[0] == 'q') && (buf[1] < 0x20) )goto finish;
	if(ncmp(buf, "exit", 4) == 0)goto finish;

	if(ncmp(buf, "say ", 4) == 0)
	{
		say("%s\n", buf+4);
	}
	else if(ncmp(buf, "win", 3) == 0)
	{
		eventwrite(
			hex32('w','i','n',0),
			hex32('w','+',0,0),
			0, 0);
	}
	else if(ncmp(buf, "ls", 2) == 0)
	{
		actorlist(0);
	}
	else if(ncmp(buf, "cd", 2) == 0)
	{
	}
	else if(ncmp(buf, "net ", 4) == 0)
	{
		netmgr_write(buf+4);
	}
	else if(ncmp(buf, "i2c ", 4) == 0)
	{
		if(ncmp(buf+4, "ls", 2) == 0)i2c_list();
		else
		{
			i2c_choose(buf+4);
		}
	}
	else
	{
		actorchoose(buf);
	}

empty:
	//command prompt
	say("[void]");
	return;

finish:
	eventwrite(0,0,0,0);
	return;
}
void term_write(u8* p)
{
	int j;
	int* enq;
	if(p == 0)return;
	//printf("%02x%02x%02x\n",p[0],p[1],p[2]);return;

	//myself
	enq = (void*)(input+0xffff0);
	while(1)
	{
		if(*p < 8)return;
		if(*p == 0x9)	//tab
		{
			say("tab");
		}
		else if((*p==0x8)|(*p==0x7f))		//backspace
		{
			say("\b \b");
			if(*enq <= 0)return;

			(*enq)--;
			input[*enq] = 0;
		}
		else if((*p==0xa)|(*p==0xd))	//enter
		{
			say("\n");
			term_read(input);

			for(j=0;j<0x100;j++)input[j] = 0;
			*enq = 0;
		}
		else
		{
			say("%c",*p);
			input[*enq] = *p;
			(*enq)++;
		}

		//////////////////
		p++;
	}
}
void term_list()
{
}
void term_choose()
{
}
void term_start()
{
}
void term_stop()
{
}
void term_create(void* addr)
{
	input = addr + 0x400000;
	output = addr + 0x500000;
}
void term_delete()
{
}
