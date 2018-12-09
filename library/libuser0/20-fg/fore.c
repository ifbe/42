#include "libuser.h"
int actorinput_editor_camera(struct arena* win, struct event* ev);
//>0x10
int actoroutput_navmenu(struct arena* win, struct style* sty);
int actorinput_navmenu( struct arena* win, struct style* sty, struct event* ev);
//>0x20
int actoroutput_tabbar(struct arena* win, struct style* sty);
int actorinput_tabbar( struct arena* win, struct style* sty, struct event* ev);
//0
int actoroutput_void(struct arena* win, struct style* sty);
int actorinput_void( struct arena* win, struct style* sty, struct event* ev);
//1
int actoroutput_console(struct arena* win, struct style* sty);
int actorinput_console( struct arena* win, struct style* sty, struct event* ev);
//2
int actoroutput_overview(struct arena* win, struct style* sty);
int actorinput_overview( struct arena* win, struct style* sty, struct event* ev);
//3
int actoroutput_detail(struct arena* win, struct style* sty);
int actorinput_detail( struct arena* win, struct style* sty, struct event* ev);
//4
int actoroutput_win(struct arena* win, struct style* sty);
int actorinput_win( struct arena* win, struct style* sty, struct event* ev);
//5
int actoroutput_2d(struct arena* win, struct style* sty);
int actorinput_2d( struct arena* win, struct style* sty, struct event* ev);
//6
int actoroutput_cad(struct arena* win, struct style* sty);
int actorinput_cad( struct arena* win, struct style* sty, struct event* ev);
//7
int actoroutput_3d(struct arena* win, struct style* sty);
int actorinput_3d( struct arena* win, struct style* sty, struct event* ev);




int fore_read(struct arena* win, struct style* sty)
{
	int ret = win->forew;
	if(ret > 0xff)say("ret=%x\n",ret);
	else if(ret >= 0x20)actoroutput_navmenu(win, 0);
	else if(ret >= 0x10)actoroutput_tabbar(win, 0);
	else if(0 == ret)actoroutput_void(win, 0);
	else if(1 == ret)actoroutput_console(win, 0);
	else if(2 == ret)actoroutput_overview(win, 0);
	else if(3 == ret)actoroutput_detail(win, 0);
	else if(4 == ret)actoroutput_win(win, 0);
	else if(5 == ret)actoroutput_2d(win, 0);
	else if(6 == ret)actoroutput_cad(win, 0);
	else if(7 == ret)actoroutput_3d(win, 0);

	return 1;
}
int fore_write(struct arena* win, struct event* ev)
{
	int ret;
	if(_coop_ == win->type)return actorinput_editor_camera(win, ev);

	ret = win->forew;
	if(ret > 0xff)say("ret=%x\n",ret);
	else if(ret >= 0x20)actorinput_navmenu(win, 0, ev);
	else if(ret >= 0x10)actorinput_tabbar(win, 0, ev);
	else if(0 == ret)actorinput_void(win, 0, ev);
	else if(1 == ret)actorinput_console(win, 0, ev);
	else if(2 == ret)actorinput_overview(win, 0, ev);
	else if(3 == ret)actorinput_detail(win, 0, ev);
	else if(4 == ret)actorinput_win(win, 0, ev);
	else if(5 == ret)actorinput_2d(win, 0, ev);
	else if(6 == ret)actorinput_cad(win, 0, ev);
	else if(7 == ret)actorinput_3d(win, 0, ev);

	return 1;
}




int foreground_start(struct arena* c, void* cf, struct arena* r, void* rf)
{
	return 0;
}
int foreground_create(struct arena* win, u8* str)
{
	return 0;
}
