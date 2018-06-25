#include "libuser.h"
#define _cam_ hex32('c','a','m',0)
#define _mic_ hex32('m','i','c',0)
#define _win_ hex32('w','i','n',0)
//
#define _HTTP_ hex32('H','T','T','P')
#define _http_ hex32('h','t','t','p')
#define _WS_ hex32('W','S',0,0)
#define _ws_ hex32('w','s',0,0)
#define _VNC_ hex32('V','N','C',0)
#define _vnc_ hex32('v','n','c',0)




void inittray(void*);
void freetray();
void initwindow(void*);
void freewindow();
void initremote(void*);
void freeremote();
//cam
int videocreate(void*);
int videodelete(void*);
int videostart(void*);
int videostop(void*);
int videoread(void* win, void* sty, void* act, void* pin);
int videowrite(void*);
int videolist();
int videochoose();
//mic
int soundcreate(void*);
int sounddelete(void*);
int soundstart(void*);
int soundstop(void*);
int soundread(void* win, void* sty, void* act, void* pin);
int soundwrite(void*);
int soundlist();
int soundchoose();
//local
int windowcreate(void*);
int windowdelete(void*);
int windowstart(void*);
int windowstop(void*);
int windowread(void* win, void* sty, void* act, void* pin);
int windowwrite(void*);
int windowlist();
int windowchoose();
//remote
int httpclient_create(void* win, u8* str);
int httpserver_create(void* win, u8* str);
int wsclient_create(void* win, u8* str);
int wsserver_create(void* win, u8* str);
int vncclient_create(void* win, u8* str);
int vncserver_create(void* win, u8* str);
//
int httpclient_delete(void* win);
int httpserver_delete(void* win);
int wsclient_delete(void* win);
int wsserver_delete(void* win);
int vncclient_delete(void* win);
int vncserver_delete(void* win);
//
int httpclient_read(void* dc,void* df,void* sc,void* sf);
int httpserver_read(void* dc,void* df,void* sc,void* sf);
int wsclient_read(void* dc, void* df, void* act, void* pin);
int wsserver_read(void* dc, void* df, void* act, void* pin);
int vncclient_read(void* dc);
int vncserver_read(void* dc);
//
int httpclient_write(void* dc,void* df,void* sc,void* sf,u8* buf,int len);
int httpserver_write(void* dc,void* df,void* sc,void* sf,u8* buf,int len);
int wsclient_write(void* dc,void* df,void* sc,void* sf,u8* buf,int len);
int wsserver_write(void* dc,void* df,void* sc,void* sf,u8* buf,int len);
int vncclient_write(void* dc,void* df,void* sc,void* sf,u8* buf,int len);
int vncserver_write(void* dc,void* df,void* sc,void* sf,u8* buf,int len);
//
int cmp(void*, void*);
int ncmp(void*, void*, int);
int parsexml_detail(void*, int, void*, void*, void*, void*);
int arenadelete(struct arena* win);




//
static struct arena* arena = 0;
static struct style* style = 0;
static int winlen = 0;
static int stylen = 0;
static int foolen = 0;
void* allocarena()
{
	int j;
	for(j=0;j<0x100;j++)
	{
		if(0 == arena[j].type)break;
	}
	if(j >= 0x100)return 0;
	return &arena[j];
}
void* allocstyle()
{
	int j,len;
	u8* buf;

	len = 0x100;	//sizeof(struct style);
	buf = (void*)style + stylen;
	stylen += len;

	for(j=0;j<len;j++)buf[j] = 0;
	return buf;
}
void* allocifoot()
{
	int j,len;
	u8* buf;

	len = 0x80;
	buf = (void*)style + 0x100000 - len - foolen;
	foolen += len;

	for(j=0;j<len;j++)buf[j] = 0;
	return buf;
}




int arenawrite_ev(struct event* ev)
{
	int j;
	void* ret;
	u64 why = ev->why;
	u64 what = ev->what;
	u64 where = ev->where;
	//say("@arenawrite:%llx,%llx,%llx\n", why, what, where);

	if(_win_ == what)
	{
		return 42;
	}
	else if(hex32('w','+',0,0) == what)
	{/*
		ret = arenacreate(why, where);
		if(ret == 0)
		{
			say("error@w+\n");
			return 0;
		}*/
	}
	else if(hex32('w','-',0,0) == what)
	{
		ret = (void*)where;
		arenadelete(ret);
	}
	return 0;
}
int arenaread_all()
{
/*
	int j;
	u64 time;
	struct relation* rel;
	struct arena* win;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	for(j=0;j<16;j++)
	{
		win = &arena[j];
		if(0 == win->type)continue;
		if(_win_ == win->type)
		{
			if(win->enq == win->deq)
			{
				sleep_us(1000);
				continue;
			}
			win->deq = win->enq;

			time = gettime();
			windowread(0, 0, win, 0);
			time = gettime() - time;
			//say("delta=%d\n",time);
		}
	}
*/
	return 0;
}




int arenawrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct arena* win;
	if(0 == dc)return arenawrite_ev(buf);

	win = dc;
	if(_HTTP_ == win->type)
	{
		httpserver_write(dc,df,sc,sf,buf,len);
	}
	else if(_http_ == win->type)
	{
		httpclient_write(dc,df,sc,sf,buf,len);
	}
	else if(_WS_ == win->type)
	{
		wsserver_write(dc,df,sc,sf,buf,len);
	}
	return 0;
}
int arenaread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	return 0;
}
int arenastop()
{
	return 0;
}
int arenastart()
{
	return 0;
}
int arenadelete(struct arena* win)
{
	if(win == 0)return 0;

	//1.close
	windowstop(win);

	//2.unlink
	win->irel0 = 0;
	win->ireln = 0;
	win->orel0 = 0;
	win->oreln = 0;

	//3.cleanup
	win->type = 0;
	win->fmt = 0;
	return 0;
}
void* arenacreate(u64 type, u8* addr)
{
	int j = 0;
	struct arena* win = allocarena();
	if(0 == win)return 0;

	win->irel0 = win->ireln = 0;
	win->orel0 = win->oreln = 0;
	if(_win_ == type)
	{
		win->type = _win_;
		win->fmt = hex64('b','g','r','a','8','8','8','8');
		windowstart(win);

		win->enq = 1;
		win->deq = 0;
		win->time = 0;
		win->fps = 60;

		win->backdata = 0;
		win->backtype = 0;
		win->modedata = 0;
		win->modetype = 2;	//mode2: overview
		win->menudata = 0;
		win->menutype = 1;	//menu1: select
		win->vkbddata = 0;
		win->vkbdtype = 0;

		//target
		win->target.vc[0] = 0.0;
		win->target.vc[1] = 0.0;
		win->target.vc[2] = 0.0;

		win->target.vr[0] = 256.0;
		win->target.vr[1] = 0.0;
		win->target.vr[2] = 0.0;

		win->target.vf[0] = 0.0;
		win->target.vf[1] = 256.0;
		win->target.vf[2] = 0.0;

		win->target.vu[0] = 0.0;
		win->target.vu[1] = 0.0;
		win->target.vu[2] = 256.0;

		//camera
		win->camera.vc[0] = 0.0;
		win->camera.vc[1] = -1024.0;
		win->camera.vc[2] = 1024.0;

		win->camera.vf[0] = (win->target.vc[0])-(win->camera.vc[0]);
		win->camera.vf[1] = (win->target.vc[1])-(win->camera.vc[1]);
		win->camera.vf[2] = (win->target.vc[2])-(win->camera.vc[2]);

		win->camera.vu[0] = 0.0;
		win->camera.vu[1] = 0.0;
		win->camera.vu[2] = 1.0;
	}
	else if(_cam_ == type)
	{
		if(0 == addr)return 0;
		win->type = _cam_;
		win->fmt = hex32('y','u','v',0);
		videostart(win);
	}
	else if(_mic_ == type)
	{
		if(0 == addr)return 0;
		win->type = _mic_;
		win->fmt = hex32('p','c','m',0);
		soundstart(win);
	}
	else if(_HTTP_ == type)
	{
		win->type = _HTTP_;
		win->fmt = hex32('h','t','m','l');

		//be server, output data
		httpserver_create(win, addr);
	}
	else if(_WS_ == type)
	{
		win->type = _WS_;
		win->fmt = hex32('j','s','o','n');

		//be server, output data
		wsserver_create(win, addr);
	}

	return win;
}
void* arenachoose(u8* buf, int len)
{
	u64 name = 0;
	int id = 0;
	u8* data = 0;
	int dl = 0;

	parsexml_detail(buf, len, &name, &id, &data, &dl);
	say("%.*s\n", len, buf);
	say("%llx, %x\n", name, id);
	say("%.*s\n", dl, data);

	if(_win_ == name)
	{
		if((id>0)&&(id<0x1000))
		{
			if(0 == arena[id].type)
			{
				arenacreate(_win_, 0);
			}
		}
	}

	return 0;
}
void* arenalist(u8* buf, int len)
{
	int j;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			if(0 == arena[j].type)break;
			say("[%03x]: %.4s,%.8s\n", j, &arena[j].type, &arena[j].fmt);
		}
		if(0 == j)say("empty arena\n");
	}
	else
	{
		for(j=0;j<len;j++){if('@' == buf[j])break;}

		j = buf[j+1]-0x30;
		if(j >= 10)j = 0;

		if(0 == arena[j].type)return 0;
		return &arena[j];
	}
	return 0;
}





void freearena()
{
	//say("[c,f):freeing arena\n");

	freewindow();
	//remotedelete();
}
void initarena(u8* addr)
{
	int j;
	arena = (void*)(addr+0x000000);
	style = (void*)(addr+0x200000);

#define max (0x100000/sizeof(struct arena))
	for(j=0;j<0x400000;j++)addr[j]=0;
	for(j=0;j<max;j++)arena[j].tier = _win_;

	inittray(arena);
	initwindow(arena);
	//initremote(arena);

	//say("[c,f):inited arena\n");
}
