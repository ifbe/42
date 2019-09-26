#include "libboot.h"
void* pwrclksearch(void*, int);
void* workersearch(void*, int);
void* devicesearch(void*, int);
void* driversearch(void*, int);
void* systemsearch(void*, int);
void* arterysearch(void*, int);
void* arenasearch(void*, int);
void* actorsearch(void*, int);
void* pwrclkmodify(int argc, void* argv);
void* workermodify(int argc, void* argv);
void* devicemodify(int argc, void* argv);
void* drivermodify(int argc, void* argv);
void* systemmodify(int argc, void* argv);
void* arterymodify(int argc, void* argv);
void* arenamodify(int argc, void* argv);
void* actormodify(int argc, void* argv);
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
		pwrclksearch(0, 0);
		say("----------------\n");
		workersearch(0, 0);
		say("----------------\n");
		devicesearch(0, 0);
		say("----------------\n");
		driversearch(0, 0);
		say("----------------\n");
		systemsearch(0, 0);
		say("----------------\n");
		arterysearch(0, 0);
		say("----------------\n");
		arenasearch(0, 0);
		say("----------------\n");
		actorsearch(0, 0);
		return;
	}

	buf += 3;
	if(0 == ncmp(buf, "pwrclk", 6))pwrclksearch(0, 0);
	else if(0 == ncmp(buf, "worker", 6))workersearch(0, 0);
	else if(0 == ncmp(buf, "device", 6))devicesearch(0, 0);
	else if(0 == ncmp(buf, "driver", 6))driversearch(0, 0);
	else if(0 == ncmp(buf, "system", 6))systemsearch(0, 0);
	else if(0 == ncmp(buf, "artery", 6))arterysearch(0, 0);
	else if(0 == ncmp(buf, "arena", 5))arenasearch(0, 0);
	else if(0 == ncmp(buf, "actor", 5))actorsearch(0, 0);
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
	else if(0 == ncmp(buf, "role", 4))
	{
		role(buf, len);
	}
/*
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
	else if(0 == ncmp(buf, "scene", 5))
	{
		scene(buf, len);
	}
*/
	else if(0 == ncmp(buf, "pwrclk", 6))pwrclkmodify(j, argv);
	else if(0 == ncmp(buf, "worker", 6))workermodify(j, argv);
	else if(0 == ncmp(buf, "device", 6))devicemodify(j, argv);
	else if(0 == ncmp(buf, "driver", 6))drivermodify(j, argv);
	else if(0 == ncmp(buf, "system", 6))systemmodify(j, argv);
	else if(0 == ncmp(buf, "artery", 6))arterymodify(j, argv);
	else if(0 == ncmp(buf, "arena", 5))arenamodify(j, argv);
	else if(0 == ncmp(buf, "actor", 5))actormodify(j, argv);

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
