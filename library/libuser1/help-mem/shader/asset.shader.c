#include "libuser.h"



int loadglslfromfile(void* url, int max, void* buf, int len)
{
    int ret = mysnprintf(buf, 99, "%s%s", GLSL_VERSION, GLSL_PRECISION);
    ret += openreadclose(url, 0, buf+ret, len-ret);
    *(u8*)(buf+ret) = 0;
    return ret;
}
int loadhlslfromfile(void* url, int max, void* buf, int len)
{
    int ret = openreadclose(url, 0, buf, len);
    *(u8*)(buf+ret) = 0;
    return ret;
}