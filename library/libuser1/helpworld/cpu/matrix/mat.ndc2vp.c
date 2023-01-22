#include "libuser.h"




//o: [ 0, 1]
//i: [-1,+1]
void uv4ndc(vec4 o, vec4 i)
{
    o[0] = i[0]/2 + 0.5;
    o[1] = i[1]/2 + 0.5;
}




//o: [-1,+1]
//i: [ 0, 1]
void ndc4uv(vec4 o, vec4 i)
{
    o[0] = i[0]*2 - 1.0;
    o[1] = i[1]*2 - 1.0;
    o[2] = 0.5;
}




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