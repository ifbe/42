#include "libuser.h"




float threecross(vec3 a, vec3 b, vec3 p)
{
    float t = (b[0]-a[0])*(p[1]-a[1]) - (p[0]-a[0])*(b[1]-a[1]);
//logtoall("t=%f\n",t);
    return t;
}
int rect_point(vec3 crf[], vec3 xyz, vec3 out[])
{
    vec3 a;
    vec3 b;
    vec3 c;
    vec3 d;
    a[0] = crf[0][0] - crf[1][0] - crf[2][0];
    a[1] = crf[0][1] - crf[1][1] - crf[2][1];
    b[0] = crf[0][0] + crf[1][0] - crf[2][0];
    b[1] = crf[0][1] + crf[1][1] - crf[2][1];
    c[0] = crf[0][0] + crf[1][0] + crf[2][0];
    c[1] = crf[0][1] + crf[1][1] + crf[2][1];
    d[0] = crf[0][0] - crf[1][0] + crf[2][0];
    d[1] = crf[0][1] - crf[1][1] + crf[2][1];
//logtoall("dbg\n");
    //(AB X AE) * (CD X CE) < 0: wrong
    if(threecross(a,b,xyz)*threecross(c,d,xyz) < 0)return 0;

    //(DA X DE) * (BC X BE) < 0: wrong
    if(threecross(d,a,xyz)*threecross(b,c,xyz) < 0)return 0;

	return 1;
}
int rect_ray(vec3 crf[], vec3 ray[], vec3 out[])
{
    return 0;
}