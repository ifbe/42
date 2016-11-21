#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




int hexstr2bigint(u8* p, u8* q)
{
        int j=0,k=0;
        while(p[j] != 0)j++;
        while(1)
        {
                //byte.low4
                j--;
                if(j < 0)break;
                if(p[j] == 0)break;
                if( (p[j] >= 0x30) && (p[j] <= 0x39) )
                {
                        q[k] = p[j] - 0x30;
                        k++;
                }
                else return 0;

                //byte.high4
                j--;
                if(j < 0)break;
                if(p[j] == 0)break;
                if( (p[j] >= 0x30) && (p[j] <= 0x39) )
                {
                        q[k-1] += (p[j]-0x30) << 4;
                }
                else return 0;
        }

        return k;
}
