#include "libuser.h"
#define _gpio_ hex32('g','p','i','o')
int boardstart(int, int);
int boardstop(int);
int boardread(int, int, void*, int);
int boardwrite(int, int, void*, int);




static int name[9] = {
	hex32('x','d', 0, 0),	//direction
	hex32('x','s', 0, 0),	//step
	hex32('y','d', 0, 0),
	hex32('y','s', 0, 0),

	hex32('z','d', 0, 0),
	hex32('z','s', 0, 0),
	hex32('w','d', 0, 0),
	hex32('w','s', 0, 0),

	hex32('e','n', 0, 0)	//enable
};
static char table[9];
static int actual[9];
static int expect[9];




static void stepcar_update(int a, int b, int c, int d)
{
	int j,k;

	actual[0] = a;
	actual[2] = b;
	actual[4] = c;
	actual[6] = d;

	for(j=0;j<8;j+=2)boardwrite(_gpio_, table[j], 0, actual[j/2]);
	for(k=0;k<200*32;k++)
	{
		for(j=1;j<8;j+=2)boardwrite(_gpio_, table[j], 0, 1);
		sleep_us(100);

		for(j=1;j<8;j+=2)boardwrite(_gpio_, table[j], 0, 0);
		sleep_us(100);
	}
}
static void stepcar_status(int EN)
{
	actual[8] = EN;
	boardwrite(_gpio_, table[8], 0, EN);
}





int stepcar_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
    int j;
    int ret = 0;

    for(j=0;j<12;j++){
        ret += mysnprintf(buf+ret, 256,
            "%.4s: pin=%d, val=%d<br>\n",
            &name[j], table[j], expect[j]
        );
    }
    return ret;
}
int stepcar_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
    printmemory(buf, len);
    return 0;
}
int stepcar_rootwrite(struct arena* win, struct style* sty, void* sc, void* sf, u8* buf, int len)
{
	int j,k;
	switch(buf[0])
	{
		case 'w':stepcar_update(0, 1, 1, 0);break;
		case 's':stepcar_update(1, 0, 0, 1);break;
		case 'a':stepcar_update(1, 1, 0, 0);break;
		case 'd':stepcar_update(0, 0, 1, 1);break;
		case 'j':stepcar_update(1, 1, 1, 1);break;
		case 'k':stepcar_update(0, 0, 0, 0);break;
		case ' ':stepcar_status(0);return 0;
		default: stepcar_status(1);return 0;
	}

	return 0;
}
int stepcar_delete(struct arena* win)
{
	int j;
	for(j=0;j<9;j++)boardstop(table[j]);
	return 0;
}
int stepcar_create(struct arena* win)
{
	int j;
	for(j=0;j<9;j++)table[j] = boardstart(name[j], 'o');
	return 0;
}
