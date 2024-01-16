#include "libsoft.h"
int decstr2double(u8*, double*);
//nema




float nema183_convert(u8* buf, int len)
{
	int t;
	double f;

	decstr2double(buf, &f);
	t = ((int)f) / 100;
	f -= 100.0*t;
	f = f/60.0 + t;

	return f;
}
int nema0183client_parse(u8* buf, int len)
{
	int j,k,t;
	double tmp;

	if(0 == ncmp(buf, "$GPRMC", 6))
	{
		logtoall("gprmc: %.*s", len, buf);

		k = 0;
		t = 0;
		for(j=0;j<len;j++)
		{
			if(',' == buf[j])
			{
				if(1 == t)logtoall("1:time=%.*s\n",            j-k-1, buf+k+1);
				else if(2 == t)logtoall("2:valid=%.*s\n",      j-k-1, buf+k+1);
				else if(3 == t)logtoall("3:latitude=%f\n",     nema183_convert(buf+k+1, j-k-1));
				else if(4 == t)logtoall("4:%.*s\n",            j-k-1, buf+k+1);
				else if(5 == t)logtoall("5:longtitude=%f\n",   nema183_convert(buf+k+1, j-k-1));
				else if(6 == t)logtoall("6:%.*s\n",            j-k-1, buf+k+1);
				else if(7 == t)logtoall("7:speed=%.*s\n",      j-k-1, buf+k+1);
				else if(8 == t)logtoall("8:angle=%.*s\n",      j-k-1, buf+k+1);
				else if(9 == t)logtoall("9:date=%.*s\n",       j-k-1, buf+k+1);
				else if(10 == t)logtoall("10:magang=%.*s\n",   j-k-1, buf+k+1);
				else if(11 == t)logtoall("11:%.*s\n",          j-k-1, buf+k+1);
				else if(12 == t)logtoall("12:mode=%.*s\n",     j-k-1, buf+k+1);
				k=j;
				t++;
			}
		}
	}
	else if(0 == ncmp(buf, "$GPVTG", 6))
	{
		logtoall("gpvtg: %.*s", len, buf);

		k = 0;
		t = 0;
		for(j=0;j<len;j++)
		{
			if(',' == buf[j])
			{
				if(1 == t)logtoall("1:rnangle=%.*s\n",      j-k-1, buf+k+1);
				else if(3 == t)logtoall("4:mnangle=%.*s\n", j-k-1, buf+k+1);
				else if(5 == t)logtoall("6:kn=%.*s\n",      j-k-1, buf+k+1);
				else if(7 == t)logtoall("7:km/h=%.*s\n",    j-k-1, buf+k+1);
				else if(9 == t)logtoall("9:mode=%.*s\n",    j-k-1, buf+k+1);
				k=j;
				t++;
			}
		}
	}
	else if(0 == ncmp(buf, "$GPGGA", 6))
	{
		logtoall("gpgga: %.*s", len, buf);

		k = 0;
		t = 0;
		for(j=0;j<len;j++)
		{
			if(',' == buf[j])
			{
				if(1 == t)logtoall("1:time=%.*s\n",            j-k-1, buf+k+1);
				else if(2 == t){
					logtoall("3:latitude=%f\n", nema183_convert(buf+k+1, j-k-1));
				}
				else if(3 == t)logtoall("4:%.*s\n",            j-k-1, buf+k+1);
				else if(4 == t){
					logtoall("5:longtitude=%f\n", nema183_convert(buf+k+1, j-k-1));
				}
				else if(5 == t)logtoall("6:%.*s\n",            j-k-1, buf+k+1);
				else if(6 == t)logtoall("2:state=%.*s\n",      j-k-1, buf+k+1);
				else if(7 == t)logtoall("7:count=%.*s\n",      j-k-1, buf+k+1);
				else if(8 == t)logtoall("8:hdop=%.*s\n",       j-k-1, buf+k+1);
				else if(9 == t){
					decstr2double(buf+k+1, &tmp);
					logtoall("9:altitude=%f\n", tmp);
				}
				else if(10 == t)logtoall("10:height=%.*s\n",   j-k-1, buf+k+1);
				else if(11 == t)logtoall("11:dt=%.*s\n",       j-k-1, buf+k+1);
				else if(12 == t)logtoall("12:id=%.*s\n",       j-k-1, buf+k+1);
				k=j;
				t++;
			}
		}
	}
	else if(0 == ncmp(buf, "$GPGSA", 6))
	{
		logtoall("gpgsa: %.*s", len, buf);

	}
	else if(0 == ncmp(buf, "$GPGSV", 6))
	{
		logtoall("gpgsv: %.*s", len, buf);

	}
	else if(0 == ncmp(buf, "$GPGLL", 6))
	{
		logtoall("gpgll: %.*s", len, buf);
	}
	else
	{
		logtoall("unknown: %.*s", len, buf);
	}
	return 0;
}




int nema0183client_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int nema0183client_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int j,k;
	struct str* str;
	_obj* ele;
	//logtoall("%.*s", len, buf);
	//printmemory(buf, len);

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	//tail
	str = ele->priv_ptr;
	if(0 != str->len)
	{
		k = str->len;
		for(j=0;j<len;j++)
		{
			str->buf[k+j] = buf[j];
			if(0xa == buf[j])
			{
				nema0183client_parse(str->buf, k+j+1);
				buf += j+1;
				len -= j+1;

				str->len = 0;
				goto freshframe;
			}
		}

		//all copyed, still going
		str->len += len;
		return 0;
	}

freshframe:
	//normal
	k = 0;
	for(j=0;j<len;j++)
	{
		if(0xa == buf[j])
		{
			nema0183client_parse(buf+k, j-k+1);
			k = j+1;
		}
	}

	//head
	if(k < len)
	{
		//printmemory(buf+k, len-k);
		for(j=0;j<len-k;j++)str->buf[j] = buf[k+j];
		str->len = len-k;
	}
	return 0;
}
int nema0183client_delete(_obj* ele)
{
	return 0;
}
int nema0183client_create(_obj* ele, u8* arg)
{
	struct str* tmp = ele->priv_ptr = memoryalloc(0x1000, 0);
	return 0;
}




int nema0183server_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int nema0183server_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int nema0183server_delete(_obj* ele, u8* arg)
{
	return 0;
}
int nema0183server_create(_obj* ele, u8* arg)
{
	return 0;
}
