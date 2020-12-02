#include "libuser.h"




//o: [w, h]
//i: [0, 1]
void xy4uv(vec4 o, vec4 i, float w, float h)
{
    o[0] = i[0] * w;
    o[1] = i[1] * h;
}




//o: [0, 1]
//i: [w, h]
void uv4xy(vec4 o, vec4 i, float w, float h)
{
    o[0] = i[0] / w;
    o[1] = i[1] / h;
}