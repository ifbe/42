#include "libuser.h"




int back_read(struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(_cli_ == fmt)return 0;
	else if(_tui_ == fmt)return 0;
	else if(_html_ == fmt)return 0;
	else if(_json_ == fmt)return 0;
	else if(_vbo_ == fmt)carveaxis(win);
	else drawaxis(win);
	return 0;
}
int back_write(struct arena* win, struct event* ev)
{
	return 0;
}