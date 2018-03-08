#include "actor.h"
int ncmp(void*, void*, int);




void parsestyle(struct style* sty, u8* buf, int len)
{
	double f;
	int j,k=0;
	for(j=0;j<len;j++)
	{
		if('\"' == buf[j])
		{
			buf += j+1;
			len -= j-1;
			break;
		}
	}
	//say("%.*s\n",len,buf);

	for(j=0;j<len;j++)
	{
		if(0 == buf[j])break;
		if(';' == buf[j])
		{
			if(0 == ncmp(buf+k, "width:", 6))
			{
				decstr2double(buf+k+6, &f);
				//say("w=%f\n", f);
			}
			else if(0 == ncmp(buf+k, "height:", 7))
			{
				decstr2double(buf+k+7, &f);
				//say("h=%f\n", f);
			}
			else if(0 == ncmp(buf+k, "left:", 5))
			{
				decstr2double(buf+k+5, &f);
				say("l=%f\n", f);

				sty->lx = f;
			}
			else if(0 == ncmp(buf+k, "right:", 6))
			{
				decstr2double(buf+k+6, &f);
				say("r=%f\n", f);

				sty->rx = f;
			}
			else if(0 == ncmp(buf+k, "top:", 4))
			{
				decstr2double(buf+k+4, &f);
				say("t=%f\n", f);

				sty->fy = f;
			}
			else if(0 == ncmp(buf+k, "bottom:", 7))
			{
				decstr2double(buf+k+7, &f);
				say("b=%f\n", f);

				sty->ny = f;
			}
			k = j+1;
		}
	}
}