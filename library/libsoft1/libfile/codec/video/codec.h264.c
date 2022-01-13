#include "libsoft.h"




int h264_headsize(u8* buf, int len)
{
    if(len <= 4)return 0;
    if((0 == buf[0])&&(0 == buf[1])){
        //case 0,0,1
        if(1 == buf[2])return 3;

        //case 0,0,0,1
        if((0 == buf[2])&&(1 == buf[3]))return 4;
    }

    //case size4b (from mp4)
    return 4;
}
int h264_ebsp2rbsp(u8* buf, int len)
{
    int type = buf[0]&0x1f;
    say("f=%d,nri=%d,type=%d\n", buf[0]>>7, (buf[0]>>5)&3, type);
    switch(type){
    case 6:say("sei\n");break;
    case 7:say("sps\n");break;
    case 8:say("pps\n");break;
    case 5:say("idr\n");break;
    case 1:say("dat\n");break;
    }
    return 0;
}




int h264_create(_obj* ele, void* url, int argc, u8** argv)
{
    return 0;
}
int h264_delete(_obj* ele, void* url)
{
    return 0;
}
int h264_attach(struct halfrel* self, struct halfrel* peer)
{
    return 0;
}
int h264_detach(struct halfrel* self, struct halfrel* peer)
{
    return 0;
}
int h264_take(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
    return 0;
}
int h264_give(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
    int headsize = h264_headsize(buf, len);
    if(headsize < 3)return 0;
    buf += headsize;
    len -= headsize;

    h264_ebsp2rbsp(buf, len);
    return 0;
}