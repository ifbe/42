#include "libsoft.h"




int solve_pcbwire(u8* buf, int w, int h, int l)
{
    int j;
    u8 bucket[256];
    for(j=0;j<256;j++)bucket[j] = 0;

    //bucket sort
    for(j=0;j<w*h*l;j++)
    {
        if(0 == buf[j])continue;
        bucket[ buf[j] ] ++;
    }

    //these wire to connect
    for(j=0;j<256;j++)
    {
        if(bucket[j] >= 2)logtoall("%d: %d\n", j, bucket[j]);
    }

    
    return 0;
}