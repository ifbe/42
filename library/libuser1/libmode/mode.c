#include "libuser.h"
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
int actoroutput_editor(struct arena* win, struct style* sty);
int actorinput_editor( struct arena* win, struct event* ev);
//5
int actoroutput_player(struct arena* win, struct style* sty);
int actorinput_player( struct arena* win, struct event* ev);




int mode_read(struct arena* win, struct style* sty)
{
	int ret = win->menutype;
	if(0 != ret)return 0;

	ret = win->modetype;
	if(0 == ret)actoroutput_void(win, 0);
	else if(1 == ret)actoroutput_console(win, 0);
	else if(2 == ret)actoroutput_overview(win, 0);
	else if(3 == ret)actoroutput_detail(win, 0);
	else if(4 == ret)actoroutput_editor(win, 0);
	else if(5 == ret)actoroutput_player(win, 0);

	return 1;
}
int mode_write(struct arena* win, struct event* ev)
{
	int ret = win->menutype;
	if(_html_ == win->fmt)
	{
		say("html input\n");
		return 0;
	}
	if(0 != ret)return 0;

	ret = win->modetype;
	if(0 == ret)actorinput_void(win, ev);
	else if(1 == ret)actorinput_console(win, ev);
	else if(2 == ret)actorinput_overview(win, ev);
	else if(3 == ret)actorinput_detail(win, ev);
	else if(4 == ret)actorinput_editor(win, ev);
	else if(5 == ret)actorinput_player(win, ev);

	return 1;
}