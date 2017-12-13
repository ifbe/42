static unsigned int seed = 0;



unsigned int getrandom()
{
	seed = seed+17;
	seed = seed*seed;
	seed = seed+41;
	seed = seed*seed*seed;
	return seed;
}
void createrandom()
{
}
void deleterandom()
{
}
