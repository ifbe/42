#include "libuser.h"




void htmlroot_parse(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<len;j++){
		if(0xa == buf[j]){
			printmemory(buf+k,j-k+1);
			k = j+1;
		}
	}
}




void background_html(struct entity* win)
{
	struct str** ctx = win->htmlctx;

	ctx[0]->len = mysnprintf(
		ctx[0]->buf, 0x10000,
		"<!DOCTYPE html>\n"
		"<html>\n"
	);
	ctx[1]->len = mysnprintf(
		ctx[1]->buf, 0x10000,
		"<head>\n"
		"<meta charset=\"UTF-8\">\n"
		"<style type=\"text/css\">\n"
		"*{margin:0;padding:0;}\n"
		"html,body{width:100%%;height:100%%;background-color:#808080;}\n"
	);
	ctx[2]->len = mysnprintf(
		ctx[2]->buf, 0x10000,
		"<body>\n"
	);
}
void foreground_html(struct entity* win)
{
	int len;
	u8* buf;
	struct str** ctx = win->htmlctx;

	len = ctx[1]->len;
	buf = ctx[1]->buf + len;
	ctx[1]->len += mysnprintf(buf, 0x10000-len, "</style>\n</head>\n");

	len = ctx[2]->len;
	buf = ctx[2]->buf + len;
	ctx[2]->len += mysnprintf(buf, 0x10000-len, "</body>\n");

	ctx[3]->len = mysnprintf(
		ctx[3]->buf, 0x10000,
		"</html>"
	);
}




int htmlroot_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void** buf,int len)
{
	say("@htmlroot_read\n");

	background_html(ent);
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dsttype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].flag = rel->dstflag;
			entityread(stack[sp+1].pchip, stack[sp+1].flag, stack,sp+2, 0,0, 0,0);
		}
		rel = samesrcnextdst(rel);
	}
	foreground_html(ent);

	int j;
	struct str** ctx = ent->htmlctx;
	for(j=0;j<4;j++)buf[j] = ctx[j];
	return 0;
}
int htmlroot_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@htmlroot_write: %.4s\n", &foot);
	if('s' == foot){	//from server, change myself
		htmlroot_parse(buf,len);
	}
	if('c' == foot){	//from client, replyto thisguy
		printmemory(buf,len);
	}
	return 0;
}
int htmlroot_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int htmlroot_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int htmlroot_delete(struct entity* win)
{
	return 0;
}
int htmlroot_create(struct entity* win, void* str)
{
	void** ctx = memorycreate(0x1000, 0);
	void*  buf = memorycreate(0x100000, 0);

	ctx[0] = buf+0x00000;
	ctx[1] = buf+0x10000;
	ctx[2] = buf+0x20000;
	ctx[3] = buf+0x30000;

	win->htmlctx = ctx;
	win->htmlbuf = buf;
	return 0;
}
