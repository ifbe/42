#include "actor.h"
//0
int actoroutput_void(struct arena* win, struct style* sty);
int actorinput_void( struct arena* win, struct event* ev);
//1
int actoroutput_console(struct arena* win, struct style* sty);
int actorinput_console( struct arena* win, struct event* ev);
//2
int actoroutput_overview(struct arena* win, struct style* sty);
int actorinput_overview( struct arena* win, struct event* ev);
//3
int actoroutput_detail(struct arena* win, struct style* sty);
int actorinput_detail( struct arena* win, struct event* ev);
//4
int actoroutput_edit(struct arena* win, struct style* sty);
int playwith2d(struct arena* win, struct event* ev);
int playwith3d(struct arena* win, struct event* ev);
//5
int actoroutput_posture(struct arena* win, struct style* sty);
int actorinput_camera( struct arena* win, struct event* ev);
//6
int actoroutput_deliver(struct arena* win, struct style* sty);
int actorinput_deliver( struct arena* win, struct event* ev);
//7
int actoroutput_oneonone(struct arena* win, struct style* sty);
int actorinput_oneonone( struct arena* win, struct event* ev);




int actoroutput_mode(struct arena* win, struct style* sty)
{
	int ret = win->menutype;
	if(0 != ret)return 0;

	ret = win->modetype;
	if(0 == ret)actoroutput_void(win, 0);
	else if(1 == ret)actoroutput_console(win, 0);
	else if(2 == ret)actoroutput_overview(win, 0);
	else if(3 == ret)actoroutput_detail(win, 0);
	else if(4 == ret)actoroutput_edit(win, 0);
	else if(5 == ret)actoroutput_posture(win, 0);
	else if(6 == ret)actoroutput_deliver(win, 0);
	else if(7 == ret)actoroutput_oneonone(win, 0);

	return 1;
}
int actorinput_mode(struct arena* win, struct event* ev)
{
	int ret = win->menutype;
	if(0 != ret)return 0;

	ret = win->modetype;
	if(0 == ret)actorinput_void(win, ev);
	else if(1 == ret)actorinput_console(win, ev);
	else if(2 == ret)actorinput_overview(win, ev);
	else if(3 == ret)actorinput_detail(win, ev);
	else if(4 == ret)
	{
		if(_vbo_ == win->fmt)playwith3d(win, ev);
		else playwith2d(win, ev);
	}
	else if(5 == ret)actorinput_camera(win, ev);
	else if(6 == ret)actorinput_deliver(win, ev);
	else if(7 == ret)actorinput_oneonone(win, ev);

	return 1;
}