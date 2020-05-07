#include "libuser.h"




void clip2ndc(vec4 o, vec4 i)
{
    float w = i[3];
    o[0] = i[0]/w;
    o[1] = i[1]/w;
    o[2] = i[2]/w;
    o[3] = 1.0;
}