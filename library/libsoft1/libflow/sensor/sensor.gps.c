#include "libsoft.h"
//nema




float gpsclient_convert(u8* buf, int len)
{
	int j,k;
	float f = 0.0;
	for(j=0;j<len;j++)
	{
		if('.' == buf[j])
		{
			//sec
			for(k=len-1;k>j;k--)
			{
				f += (buf[k]-48.0);
				f /= 10;
			}
			f /= 3600.0;

			//min
			f += ((buf[j-2]-48.0)*10 + buf[j-1]-48.0)/ 60.0;

			//deg
			if(j <= 4)f += (buf[0]-48.0)*10 + (buf[1]-48.0);
			else f += (buf[0]-48.0)*100 + (buf[1]-48.0)*10 + (buf[2]-48.0);
		}
	}
	return f;
}
int gpsclient_parse(u8* buf, int len)
{
	int j,k,t;
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
				else if(3 == t)say("3:latitude=%f\n",     gpsclient_convert(buf+k+1, j-k-1));
				else if(4 == t)say("4:%.*s\n",            j-k-1, buf+k+1);
				else if(5 == t)say("5:longtitude=%f\n",   gpsclient_convert(buf+k+1, j-k-1));
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
				else if(2 == t)say("3:latitude=%.*s\n",   j-k-1, buf+k+1);
				else if(3 == t)say("4:%.*s\n",            j-k-1, buf+k+1);
				else if(4 == t)say("5:longtitude=%.*s\n", j-k-1, buf+k+1);
				else if(5 == t)say("6:%.*s\n",            j-k-1, buf+k+1);
				else if(6 == t)say("2:state=%.*s\n",      j-k-1, buf+k+1);
				else if(7 == t)say("7:count=%.*s\n",      j-k-1, buf+k+1);
				else if(8 == t)say("8:hdop=%.*s\n",       j-k-1, buf+k+1);
				else if(9 == t)say("9:altitude=%.*s\n",   j-k-1, buf+k+1);
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
}
int gpsclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int j,k;
	struct str* str;
	//say("%.*s", len, buf);
	//printmemory(buf, len);

	//tail
	str = ele->buf;
	if(0 != str->len)
	{
		k = str->len;
		for(j=0;j<len;j++)
		{
			str->buf[k+j] = buf[j];
			if(0xa == buf[j])
			{
				gpsclient_parse(str->buf, k+j+1);
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
			gpsclient_parse(buf+k, j-k+1);
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
int gpsclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	struct str* str;
	struct object* obj = systemcreate(_uart_, url);
	if(0 == obj)return 0;

	ele->buf = memorycreate(0x1000);
	str = ele->buf;
	str->len = 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}