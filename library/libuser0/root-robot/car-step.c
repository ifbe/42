#include "libuser.h"
#define _gpio_ hex32('g','p','i','o')
int boardstart(int, int);
int boardstop(int);
int boardread(int, int, void*, int);
int boardwrite(int, int, void*, int);




int stepcar_rootread(struct arena* win, struct style* sty, void* sc, void* sf, u8* buf, int len)
{
    return 0;
}
int stepcar_rootwrite(struct arena* win, struct style* sty, void* sc, void* sf, u8* buf, int len)
{
    return 0;
}
int stepcar_stop(struct arena* win, struct style* sty)
{
    return 0;
}
int stepcar_start(struct arena* win, struct style* sty)
{
    return 0;
}
int stepcar_delete(struct arena* win)
{
    return 0;
}
int stepcar_create(struct arena* win)
{
    return 0;
}