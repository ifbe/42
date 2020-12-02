#include "libuser.h"




void clip2ndc(vec4 o, vec4 i)
{
    float invw = 1.0/i[3];
    o[0] = i[0] * invw;
    o[1] = i[1] * invw;
    o[2] = i[2] * invw;
    o[3] = 1.0;
}