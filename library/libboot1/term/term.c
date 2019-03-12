#include "libboot.h"
void* devicelist(void*, int);
void* driverlist(void*, int);
void* systemlist(void*, int);
void* arterylist(void*, int);
void* arenalist(void*, int);
void* actorlist(void*, int);
void* devicecommand(int argc, void* argv);
void* drivercommand(int argc, void* argv);
void* systemcommand(int argc, void* argv);
void* arterycommand(int argc, void* argv);
void* arenacommand(int argc, void* argv);
void* actorcommand(int argc, void* argv);
//
int event(void*, int);
int node(void*, int);
int relation(void*, int);
int role(void*, int);
int scene(void*, int);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);
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
		devicelist(0, 0);
		say("----------------\n");
		driverlist(0, 0);
		say("----------------\n");
		systemlist(0, 0);
		say("----------------\n");
		arterylist(0, 0);
		say("----------------\n");
		arenalist(0, 0);
		say("----------------\n");
		actorlist(0, 0);
		return;
	}

	buf += 3;
	if(0 == ncmp(buf, "device", 6))devicelist(0, 0);
	else if(0 == ncmp(buf, "driver", 6))driverlist(0, 0);
	else if(0 == ncmp(buf, "system", 6))systemlist(0, 0);
	else if(0 == ncmp(buf, "artery", 6))arterylist(0, 0);
	else if(0 == ncmp(buf, "arena", 5))arenalist(0, 0);
	else if(0 == ncmp(buf, "actor", 5))actorlist(0, 0);
	else say("ls(%s)\n", buf);
}




void term_prompt()
{
	say("[term@void]");
}
int termwrite(u8* buf, int len)
{
	int j;
	u8* argv[8];
	u8 tmp[0x1000];
	if(0 == buf)goto finish;
	if(buf[0] <= 0x20)goto finish;

	if(('q' == buf[0])&&(buf[1] <= 0x20))goto byebye;
	if(0 == ncmp(buf, "exit", 4))goto byebye;

	j = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(0 == ncmp(buf, "ls", 2))
	{
		term_ls(buf, len);
	}
	else if(0 == ncmp(buf, "ev", 2))
	{
		event(buf, len);
	}
	else if(0 == ncmp(buf, "node", 4))
	{
		node(buf, len);
	}
	else if(0 == ncmp(buf, "rel", 3))
	{
		relation(buf, len);
	}
	else if(0 == ncmp(buf, "role", 4))
	{
		role(buf, len);
	}
	else if(0 == ncmp(buf, "scene", 5))
	{
		scene(buf, len);
	}
	else if(0 == ncmp(buf, "device", 6))devicecommand(j, argv);
	else if(0 == ncmp(buf, "driver", 6))drivercommand(j, argv);
	else if(0 == ncmp(buf, "system", 6))systemcommand(j, argv);
	else if(0 == ncmp(buf, "artery", 6))arterycommand(j, argv);
	else if(0 == ncmp(buf, "arena", 5))arenacommand(j, argv);
	else if(0 == ncmp(buf, "actor", 5))actorcommand(j, argv);

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