#include "libuser.h"
int actoroutput_navmenu(struct arena* win, struct style* sty);
int actorinput_navmenu( struct arena* win, struct event* ev);




int actoroutput_menu(struct arena* win, struct style* sty)
{
	int ret = win->menutype;
	if(0 == ret)return 0;

	if(1 == ret)actoroutput_navmenu(win, 0);
	else if(2 == ret)say("menu=2\n");
	else if(3 == ret)say("menu=3\n");

	return 1;
}
int actorinput_menu(struct arena* win, struct event* ev)
{
	int ret = win->menutype;
	if(0 == ret)return 0;

	actorinput_navmenu(win, ev);
	return 1;
}