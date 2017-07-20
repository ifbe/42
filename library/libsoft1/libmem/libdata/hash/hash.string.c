#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




u32 bkdrhash(u8* buf, int len)
{
	int j;
	u32 hash = 0;
	for(j=0;j<len;j++)
	{
		hash = hash * 131 + buf[j];
	}
	return hash;
}
u32 aphash(u8* buf, int len)
{
	int j;
	u32 hash = 0;
	for(j=0;j<len;j++)
	{
		if ((j & 1) == 0)
		{
			hash ^= ((hash << 7) ^ buf[j] ^ (hash >> 3));
		}
		else
		{
			hash ^= (~((hash << 11) ^ buf[j] ^ (hash >> 5)));
		}
	}
	return hash;
}
u32 djb2hash(u8* buf, int len)
{
	int j;
	u32 hash = 5381;
	for(j=0;j<len;j++)
	{
		hash += (hash<<5) + hash + buf[j];
	}
	return hash;
}
u32 jshash(u8* buf, int len)
{
	int j;
	u32 hash = 1315423911;
	for(j=0;j<len;j++)
	{
		hash ^= (hash << 5) + (hash >> 2) + buf[j];
	}
	return hash;
}
u32 rshash(u8* buf, int len)
{
	int j;
	u32 a = 63689;
	u32 b = 378551;
	u32 hash = 0;
	for(j=0;j<len;j++)
	{
		hash = hash * a + buf[j];
		a *= b;
	}
	return hash;
}
u32 sdbmhash(u8* buf, int len)
{
	int j;
	u32 hash = 0;
	for(j=0;j<len;j++)
	{
		// equivalent to: hash = 65599*hash + (*str++);
		hash = buf[j] + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}
u32 pjwhash(u8* buf, int len)
{
	u32 BitsInUnignedInt = (u32)(sizeof(u32) * 8);
	u32 ThreeQuarters    = (u32)(BitsInUnignedInt * 3 / 4);
	u32 OneEighth        = (u32)(BitsInUnignedInt / 8);
	u32 HighBits         = (u32)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
	u32 hash             = 0;
	u32 test             = 0;
	int j;
	for(j=0;j<len;j++)
	{
		hash = (hash << OneEighth) + buf[j];
		if ((test = hash & HighBits) != 0)
		{
			hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}
	return hash;
}
u32 elfhash(u8* buf, int len)
{
	int j;
	u32 x = 0;
	u32 hash = 0;
	for(j=0;j<len;j++)
	{
		hash = (hash << 4) + buf[j];
		if ((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}
	return hash;
}
