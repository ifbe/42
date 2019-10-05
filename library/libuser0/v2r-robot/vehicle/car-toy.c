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
static void toycar_update(int L, int R, int el, int er)
{
    int j;

    value[0] = value[2] = L;
    value[1] = value[3] = !L;

    value[4] = value[6] = R;
    value[5] = value[7] = !R;

    value[ 8] = value[ 9] = el;
    value[10] = value[11] = er;

    for(j=0;j<12;j++)boardwrite(_gpio_, table[j], 0, value[j]);
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
    //printmemory(buf, len);

    switch(buf[0])
    {
	case '1':boardwrite(_gpio_, table[ 8], 0, 1);break;
	case '2':boardwrite(_gpio_, table[ 9], 0, 1);break;
	case '3':boardwrite(_gpio_, table[10], 0, 1);break;
	case '4':boardwrite(_gpio_, table[11], 0, 1);break;

        case 'a':toycar_update(1, 0, 1, 1);break;
        case 'd':toycar_update(0, 1, 1, 1);break;
        case 'w':toycar_update(1, 1, 1, 1);break;
        case 's':toycar_update(0, 0, 1, 1);break;

        case 'q':toycar_update(1, 1, 0, 1);break;
        case 'e':toycar_update(1, 1, 1, 0);break;
        case 'z':toycar_update(0, 0, 0, 1);break;
        case 'c':toycar_update(0, 0, 1, 0);break;

        default: toycar_update(0, 0, 0, 0);break;
    }
    return 0;
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
