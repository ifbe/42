void term_create(void*);
void wmgr_create(void*);




void helper_create(void* addr)
{
	term_create(addr);
	wmgr_create(addr);
}
void helper_delete()
{
}