unsigned short bswap16(unsigned short old)
{
	//not supported
	//return __builtin_bswap16(old);

	return ((old&0xff00)>>8) + ((old&0xff)<<8);
}
unsigned int bswap32(unsigned int old)
{
	return __builtin_bswap32(old);
}
unsigned long long bswap64(unsigned long long old)
{
	return __builtin_bswap64(old);
}
