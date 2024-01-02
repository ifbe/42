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
		say("gprmc: %.*s", len, buf);

		k = 0;
		t = 0;
		for(j=0;j<len;j++)
		{
			if(',' == buf[j])
			{
				if(1 == t)say("1:time=%.*s\n",            j-k-1, buf+k+1);
				else if(2 == t)say("2:valid=%.*s\n",      j-k-1, buf+k+1);
				else if(3 == t)say("3:latitude=%f\n",     nema183_convert(buf+k+1, j-k-1));
				else if(4 == t)say("4:%.*s\n",            j-k-1, buf+k+1);
				else if(5 == t)say("5:longtitude=%f\n",   nema183_convert(buf+k+1, j-k-1));
				else if(6 == t)say("6:%.*s\n",            j-k-1, buf+k+1);
				else if(7 == t)say("7:speed=%.*s\n",      j-k-1, buf+k+1);
				else if(8 == t)say("8:angle=%.*s\n",      j-k-1, buf+k+1);
				else if(9 == t)say("9:date=%.*s\n",       j-k-1, buf+k+1);
				else if(10 == t)say("10:magang=%.*s\n",   j-k-1, buf+k+1);
				else if(11 == t)say("11:%.*s\n",          j-k-1, buf+k+1);
				else if(12 == t)say("12:mode=%.*s\n",     j-k-1, buf+k+1);
				k=j;
				t++;
			}
		}
	}
	else if(0 == ncmp(buf, "$GPVTG", 6))
	{
		say("gpvtg: %.*s", len, buf);

		k = 0;
		t = 0;
		for(j=0;j<len;j++)
		{
			if(',' == buf[j])
			{
				if(1 == t)say("1:rnangle=%.*s\n",      j-k-1, buf+k+1);
				else if(3 == t)say("4:mnangle=%.*s\n", j-k-1, buf+k+1);
				else if(5 == t)say("6:kn=%.*s\n",      j-k-1, buf+k+1);
				else if(7 == t)say("7:km/h=%.*s\n",    j-k-1, buf+k+1);
				else if(9 == t)say("9:mode=%.*s\n",    j-k-1, buf+k+1);
				k=j;
				t++;
			}
		}
	}
	else if(0 == ncmp(buf, "$GPGGA", 6))
	{
		say("gpgga: %.*s", len, buf);

		k = 0;
		t = 0;
		for(j=0;j<len;j++)
		{
			if(',' == buf[j])
			{
				if(1 == t)say("1:time=%.*s\n",            j-k-1, buf+k+1);
				else if(2 == t){
					say("3:latitude=%f\n", nema183_convert(buf+k+1, j-k-1));
				}
				else if(3 == t)say("4:%.*s\n",            j-k-1, buf+k+1);
				else if(4 == t){
					say("5:longtitude=%f\n", nema183_convert(buf+k+1, j-k-1));
				}
				else if(5 == t)say("6:%.*s\n",            j-k-1, buf+k+1);
				else if(6 == t)say("2:state=%.*s\n",      j-k-1, buf+k+1);
				else if(7 == t)say("7:count=%.*s\n",      j-k-1, buf+k+1);
				else if(8 == t)say("8:hdop=%.*s\n",       j-k-1, buf+k+1);
				else if(9 == t){
					decstr2double(buf+k+1, &tmp);
					say("9:altitude=%f\n", tmp);
				}
				else if(10 == t)say("10:height=%.*s\n",   j-k-1, buf+k+1);
				else if(11 == t)say("11:dt=%.*s\n",       j-k-1, buf+k+1);
				else if(12 == t)say("12:id=%.*s\n",       j-k-1, buf+k+1);
				k=j;
				t++;
			}
		}
	}
	else if(0 == ncmp(buf, "$GPGSA", 6))
	{
		say("gpgsa: %.*s", len, buf);

	}
	else if(0 == ncmp(buf, "$GPGSV", 6))
	{
		say("gpgsv: %.*s", len, buf);

	}
	else if(0 == ncmp(buf, "$GPGLL", 6))
	{
		say("gpgll: %.*s", len, buf);
	}
	else
	{
		say("unknown: %.*s", len, buf);
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
	//say("%.*s", len, buf);
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
