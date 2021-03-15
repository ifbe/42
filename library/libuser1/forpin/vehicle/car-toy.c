#include "libuser.h"
#define _gpio_ hex32('g','p','i','o')
int gpiostart(int, int);
int gpiostop(int);
int gpioread(int, int, void*, int);
int gpiowrite(int, int, void*, int);




static int name[16] = {
	hex32('l','f','p',0),
	hex32('l','f','n',0),
	hex32('l','n','p',0),
	hex32('l','n','n',0),

	hex32('r','f','p',0),
	hex32('r','f','n',0),
	hex32('r','n','p',0),
	hex32('r','n','n',0),

	hex32('l','f','e',0),
	hex32('l','n','e',0),
	hex32('r','f','e',0),
	hex32('r','n','e',0)
};
static char table[16];
static char value[16];
static void toycar_update(int L, int R, int el, int er)
{
	int j;

	value[0] = value[2] = L;
	value[1] = value[3] = !L;

	value[4] = value[6] = R;
	value[5] = value[7] = !R;

	value[ 8] = value[ 9] = el;
	value[10] = value[11] = er;

	for(j=0;j<12;j++)gpiowrite(_gpio_, table[j], 0, value[j]);
}




int toycar_read_byhttp(struct entity* ent,void* foot, struct halfrel* stack,int sp)
{
	u8 buf[1024];
	int j;
	int ret = mysnprintf(buf, 999,
	"<html><body>"
	"<form method=\"post\">"
	"<input type=\"text\" name=\"fuck\">"
	"</form>"
	);

	for(j=0;j<12;j++){
	ret += mysnprintf(buf+ret, 999-ret,
		"%.4s: pin=%d, val=%d<br>\n",
		&name[j], table[j], value[j]
	);
	}

	ret += mysnprintf(buf+ret, 999-ret, "</body></html>");
	give_data_into_peer(ent,stack[sp-1].flag, stack,sp, "text/html",0, buf,ret);
	return ret;
}
int toycar_taking(struct entity* ent,void* foot, struct halfrel* stack,int sp, void* arg,int key, u8* buf,int len)
{
	say("@toycar_read:%p,%p,sp=%x\n", ent,foot, sp);
	if(0 == stack)return 0;

	struct entity* caller = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _http_:
	case _HTTP_:
		return toycar_read_byhttp(ent,foot, stack,sp);
	}

	return 0;
}
int toycar_giving(struct entity* ent,void* foot, struct halfrel* stack,int sp, void* arg,int key, u8* buf,int len)
{
	say("@toycar_write\n");
	//printmemory(buf, len);

	//do work
	switch(buf[0])
	{
	case '1':gpiowrite(_gpio_, table[ 8], 0, 1);break;
	case '2':gpiowrite(_gpio_, table[ 9], 0, 1);break;
	case '3':gpiowrite(_gpio_, table[10], 0, 1);break;
	case '4':gpiowrite(_gpio_, table[11], 0, 1);break;

	case 'a':toycar_update(1, 0, 1, 1);break;
	case 'd':toycar_update(0, 1, 1, 1);break;
	case 'w':toycar_update(1, 1, 1, 1);break;
	case 's':toycar_update(0, 0, 1, 1);break;

	case 'q':toycar_update(1, 1, 0, 1);break;
	case 'e':toycar_update(1, 1, 1, 0);break;
	case 'z':toycar_update(0, 0, 0, 1);break;
	case 'c':toycar_update(0, 0, 1, 0);break;

	default: toycar_update(0, 0, 0, 0);break;
	}

	//send back: my state
	int j,ret=0;
	u8 tmp[256];

	for(j=0;j<12;j++)ret += mysnprintf(tmp+ret, 256-ret,"%.4s=%d,",&name[j], value[j]);
	tmp[ret] = '\n';
	ret++;

	give_data_into_peer(ent,stack[sp-1].flag, stack,sp, 0,0, tmp,ret);
	return 0;
}
void toycar_discon(struct halfrel* self, struct halfrel* peer)
{
}
void toycar_linkup(struct halfrel* self, struct halfrel* peer)
{
}
void toycar_delete(struct supply* win)
{
	int j;
	for(j=0;j<12;j++)gpiostop(table[j]);
}
void toycar_create(struct supply* win)
{
	int j;
	for(j=0;j<12;j++)table[j] = gpiostart(name[j], 'o');
}
