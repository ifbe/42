#include "libuser.h"
#define _chip_ hex32('c','h','i','p')
#define _vsrc_ hex32('v','s','r','c')
#define _resistor_ hex64('r','e','s','i','s','t','o','r')




int pchip_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
    struct relation* rel;
    struct actor* act;
    if(0 == twig)return 0;

say("@pchip_start{\n");
    rel = twig->orel0;
    while(1)
    {
        if(0 == rel)break;

        if(_act_ == rel->dsttype)
        {
            act = (void*)(rel->dstchip);
            say("%llx:%.8s\n", act, &act->name);
        }

        rel = samesrcnextdst(rel);
    }
say("}\n");
    return 0;
    return 0;
}
int pchip_create(struct arena* win, u8* str)
{
	struct actor* c0;
	struct actor* c1;

    c0 = actorcreate(_vsrc_, 0);
    if(c0)relationcreate(c0, 0, _act_, win, 0, _win_);

    c1 = actorcreate(_resistor_, 0);
    if(c1)relationcreate(c1, 0, _act_, win, 0, _win_);

    return 0;
}