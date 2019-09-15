#include "libuser.h"
#define _gpio_ hex32('g','p','i','o')
int boardstart(int, int);
int boardstop(int);
int boardread(int, int, void*, int);
int boardwrite(int, int, void*, int);




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
static void toycar_update(int L, int R)
{
    int j;

    value[0] = value[2] = L;
    value[1] = value[3] = !L;

    value[4] = value[6] = R;
    value[5] = value[7] = !R;

    for(j=0;j<8;j++)boardwrite(_gpio_, table[j], 0, value[j]);
}
static void toycar_status(int E)
{
    int j;
    for(j=8;j<12;j++)value[j] = E;
    for(j=8;j<12;j++)boardwrite(_gpio_, table[j], 0, value[j]);
}




int toycar_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
    int j;
    int ret = mysnprintf(buf, 256,
        "<html><body>"
        "<form method=\"post\">"
        "<input type=\"text\" name=\"fuck\">"
        "</form>"
    );

    for(j=0;j<12;j++){
        ret += mysnprintf(buf+ret, 256,
            "%.4s: pin=%d, val=%d<br>\n",
            &name[j], table[j], value[j]
        );
    }

    ret += mysnprintf(buf+ret, 256, "</body></html>");
    return ret;
}
int toycar_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
    say("@toycar_write\n");
    printmemory(buf, len);
    return 0;
}
void toycar_rootwrite(struct arena* win, struct style* sty, void* sc, void* sf, u8* buf, int len)
{
    if(0 == buf)return;

    switch(buf[0])
    {
        case 'a':toycar_update(1, 0);break;
        case 'd':toycar_update(0, 1);break;
        case 'w':toycar_update(1, 1);break;
        case 's':toycar_update(0, 0);break;
        case ' ':toycar_status(1);break;
        default: toycar_status(0);break;
    }
}
void toycar_delete(struct arena* win)
{
    int j;
    for(j=0;j<12;j++)boardstop(table[j]);
}
void toycar_create(struct arena* win)
{
    int j;
    for(j=0;j<12;j++)table[j] = boardstart(name[j], 'o');
}
