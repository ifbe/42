#include "libhard.h"
void out16(u16,u16);
void getportanddata(u16*,u16*);
void poweroff()
{
    u16 port,data;
    getportanddata(&port,&data);
    if(0 == port){
        say("badport@poweroff:%x\n",port);
        return;
    }

    out16(port, data|0x2000);
}