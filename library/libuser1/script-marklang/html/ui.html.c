#include "libuser.h"
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_nocam(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_whcam(_obj* wnd, struct style* area);




struct httpparsed{
	u8* GET;
	u8* POST;
	u8* Host;
	u8* Connection;
	u8* Upgrade;
	u8* Content_Length;
	u8* Content_Type;
	u8* Content;
	u8* End;
};




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




void background_html(_obj* win)
{
	struct str** ctx = win->html.ctx;

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
void foreground_html(_obj* win)
{
	int len;
	u8* buf;
	struct str** ctx = win->html.ctx;

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




int htmlroot_taketext(_obj* ent,void* foot, _syn* stack,int sp)
{
	say("@htmlroot_read\n");

	background_html(ent);
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dstnodetype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].foottype = rel->srcfoottype;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].foottype = rel->dstfoottype;
			entity_takeby(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, 0,0, 0,0);
		}
		rel = samesrcnextdst(rel);
	}
	foreground_html(ent);
	return 0;
}
int htmlroot_sendtext(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void** buf,int len)
{
	int j;
	struct str** ctx = ent->html.ctx;

	struct httpparsed head;
	head.Content_Length = 0;
	head.Content_Type = (u8*)"text/html";
	for(j=0;j<4;j++)head.Content_Length += ctx[j]->len;

	for(j=0;j<4;j++){
		give_data_into_peer(ent,_src_, stack,sp, (p64)&head,j, ctx[j]->buf,ctx[j]->len);
	}
	return 0;
}




int htmlroot_draw_gl41(
	_obj* ent, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
{
	return 0;
}
int htmlroot_draw_gl41_whcam(
	_obj* ent, struct style* part,
	_obj* wnd, struct style* area)
{
	int j;
	struct fstyle fs;
	for(j=0;j<3;j++)fs.vc[j] = fs.vr[j] = fs.vf[j] = fs.vt[j] = 0.0;
	fs.vc[0] = (area->fs.vc[0]+area->fs.vq[0]) * wnd->whdf.fbwidth / 2.0;
	fs.vc[1] = (area->fs.vc[1]+area->fs.vq[1]) * wnd->whdf.fbheight / 2.0;
	fs.vr[0] = (area->fs.vq[0]-area->fs.vc[0]) * wnd->whdf.fbwidth / 2.0;
	fs.vf[1] = (area->fs.vq[1]-area->fs.vc[1]) * wnd->whdf.fbheight/ 2.0;

	gl41data_before(wnd);
	gl41data_whcam(wnd, area);

	htmlroot_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_after(wnd);
	return 0;
}




int htmlroot_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void** buf,int len)
{
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	say("caller:type=%.4s,fmt=%.4s\n", &caller->type, &caller->hfmt);

	//slot type known: do work based on slot type
	//switch(stack[sp-1].foottype){
	//}

	//slot type unknown: do work based on caller fmt
	switch(caller->hfmt){
	case _gl41list_:
		htmlroot_taketext(ent,slot, stack,sp);
		htmlroot_draw_gl41_whcam(ent,slot, caller,area);
		break;
	case _http_:
		say("byhttp\n");
		break;
	}
	return 0;
}
int htmlroot_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//say("@htmlroot_write: %.4s\n", &foot);
	_obj* caller = stack[sp-2].pchip;
	say("caller:type=%.4s,fmt=%.4s\n", &caller->type, &caller->hfmt);

	//slot type known: do work based on slot type
	switch(stack[sp-1].foottype){
	case 's':	//from server, change myself
		htmlroot_parse(buf,len);
		return 0;
	case 'c':	//from client, replyto thisguy
		printmemory(buf,len);
		return 0;
	}

	//slot type unknown: do work based on caller fmt
	switch(caller->type){
	case _HTTP_:
		htmlroot_taketext(ent,foot, stack,sp);
		htmlroot_sendtext(ent,foot, stack,sp, arg,key, buf,len);
		break;
	case _http_:
		break;
	}
	return 0;
}
int htmlroot_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int htmlroot_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int htmlroot_delete(_obj* win)
{
	return 0;
}
int htmlroot_create(_obj* win, void* str)
{
	void** ctx = memoryalloc(0x1000, 0);
	void*  buf = memoryalloc(0x100000, 0);

	ctx[0] = buf+0x00000;
	ctx[1] = buf+0x10000;
	ctx[2] = buf+0x20000;
	ctx[3] = buf+0x30000;

	win->html.ctx = ctx;
	win->html.buf = buf;
	return 0;
}
