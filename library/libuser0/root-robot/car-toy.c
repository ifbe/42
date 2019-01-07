#include "libuser.h"
#define _gpio_ hex32('g','p','i','o')
int boardstart(int, int);
int boardstop(int);
int boardread(int, int, void*, int);
int boardwrite(int, int, void*, int);




int name[16] = {
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
char table[16];




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
void toycar_stop(struct arena* win)
{
    int j;
    for(j=8;j<12;j++)boardwrite(_gpio_, table[j], 0, 0);
}
void toycar_start(struct arena* win)
{
    int j;
    for(j=8;j<12;j++)boardwrite(_gpio_, table[j], 0, 1);
}
void toycar_rootread(struct arena* win, struct style* sty, void* sc, void* sf, u8* buf, int len)
{
}
void toycar_rootwrite(struct arena* win, struct style* sty, void* sc, void* sf, u8* buf, int len)
{
    int L,R;
    if(0 == buf)return;

    switch(buf[0])
    {
        case 'l':L = 1;R = 0;break;
        case 'r':L = 0;R = 1;break;
        case 'f':L = 1;R = 1;break;
        case 'n':L = 0;R = 0;break;
        case 0x20:toycar_stop(win);return;
        default:  toycar_stop(win);return;
    }

	boardwrite(_gpio_, table[0], 0, L);
	boardwrite(_gpio_, table[1], 0, !L);
	boardwrite(_gpio_, table[2], 0, L);
	boardwrite(_gpio_, table[3], 0, !L);

	boardwrite(_gpio_, table[4], 0, R);
	boardwrite(_gpio_, table[5], 0, !R);
	boardwrite(_gpio_, table[6], 0, R);
	boardwrite(_gpio_, table[7], 0, !R);
}