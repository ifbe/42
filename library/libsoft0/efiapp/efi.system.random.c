static unsigned int seed = 0;



unsigned int random_read()
{
	seed = 1103515245*seed + 12345;
	return seed;
}
void createrandom()
{
}
void deleterandom()
{
}


void freerandom()
{
}
void initrandom()
{
}
