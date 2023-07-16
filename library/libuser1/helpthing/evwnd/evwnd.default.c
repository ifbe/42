#include "libuser.h"




int windowevent_take(_obj* wnd,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
    struct event* ev = buf;
    return 0;
}
int windowevent_give(_obj* wnd,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
    struct event* ev = buf;
    return 0;
}




int windowevent_reader(_obj* wnd,void* foot, _syn* stack,int sp, void* buf,int len)
{
    struct event* ev = buf;
    return 0;
}
int windowevent_writer(_obj* wnd,void* foot, _syn* stack,int sp, void* buf,int len)
{
    struct event* ev = buf;
    return 0;
}