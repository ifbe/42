void term_create(void*);
void connect_create(void*);




void helper_create(void* addr)
{
	term_create(addr);
	connect_create(addr);
}
void helper_delete()
{
}