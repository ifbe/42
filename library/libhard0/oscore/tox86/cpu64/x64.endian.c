unsigned short bswap16(unsigned short old)
{
	return __builtin_bswap16(old);
}
unsigned int bswap32(unsigned int old)
{
	return __builtin_bswap32(old);
}
unsigned long long bswap64(unsigned long long old)
{
	return __builtin_bswap64(old);
}
