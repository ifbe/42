#include "libboot.h"
#define _device_ hex64('d','e','v','i','c','e',0,0)
#define _driver_ hex64('d','r','i','v','e','r',0,0)
#define _system_ hex64('s','y','s','t','e','m',0,0)
#define _artery_ hex64('a','r','t','e','r','y',0,0)
#define _arena_ hex64('a','r','e','n','a',0,0,0)
#define _actor_ hex64('a','c','t','o','r',0,0,0)
//
void* actorget(void*, int);
int actorpost(void*, int);
void* arenaget(void*, int);
int arenapost(void*, int);
void* arteryget(void*, int);
int arterypost(void*, int);
void* systemget(void*, int);
int systempost(void*, int);
void* driverget(void*, int);
int driverpost(void*, int);
void* deviceget(void*, int);
int devicepost(void*, int);
//
int scene_recover(void*, int);
int scene_backup(void*, int);
int wire_add(void*, int);
int wire_del(void*, int);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




void term_ls(u8* buf, int len)
{
	if(buf[3] <= 0x20)
	{
		say("----------------\n");
		deviceget(0, 0);
		say("----------------\n");
		driverget(0, 0);
		say("----------------\n");
		systemget(0, 0);
		say("----------------\n");
		arteryget(0, 0);
		say("----------------\n");
		arenaget(0, 0);
		say("----------------\n");
		actorget(0, 0);
		return;
	}

	buf += 3;
	if(0 == ncmp(buf, "device", 6))deviceget(0, 0);
	else if(0 == ncmp(buf, "driver", 6))driverget(0, 0);
	else if(0 == ncmp(buf, "system", 6))systemget(0, 0);
	else if(0 == ncmp(buf, "artery", 6))arteryget(0, 0);
	else if(0 == ncmp(buf, "arena", 5))arenaget(0, 0);
	else if(0 == ncmp(buf, "actor", 5))actorget(0, 0);
	else say("ls(%s)\n", buf);
}




void term_prompt()
{
	say("[term@void]");
}
int termwrite(u8* buf, int len)
{
	int j;
	if(0 == buf)goto finish;
	if(buf[0] <= 0x20)goto finish;

	if(('q' == buf[0])&&(buf[1] <= 0x20))goto byebye;
	if(0 == ncmp(buf, "exit", 4))goto byebye;

	//
	if(0 == ncmp(buf, "ls", 2))
	{
		term_ls(buf, len);
	}
	else if(0 == ncmp(buf, "event", 5))
	{
		say("%.*s\n", len, buf);
	}
	else if(0 == ncmp(buf, "backup", 6))
	{
		scene_backup(buf+6, 0);
	}
	else if(0 == ncmp(buf, "recover", 7))
	{
		scene_recover(buf+7, 0);
	}
	else if(0 == ncmp(buf, "wireadd", 7))
	{
		wire_add(buf+7, 0);
	}
	else if(0 == ncmp(buf, "wiredel", 7))
	{
		wire_add(buf+7, 0);
	}
	else if(0 == ncmp(buf, "device", 6))devicepost(buf+7, 0);
	else if(0 == ncmp(buf, "driver", 6))driverpost(buf+7, 0);
	else if(0 == ncmp(buf, "system", 6))systempost(buf+7, 0);
	else if(0 == ncmp(buf, "artery", 6))arterypost(buf+7, 0);
	else if(0 == ncmp(buf, "arena", 5))arenapost(buf+6, 0);
	else if(0 == ncmp(buf, "actor", 5))actorpost(buf+6, 0);

finish:
	term_prompt();
	return 1;

byebye:
	eventwrite(0,0,0,0);
	return -1;
}
void termread()
{
}