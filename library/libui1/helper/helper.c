void login_create(void*);
void term_create(void*);
void wmgr_create(void*);




void helper_create(void* addr)
{
	login_create(addr);
	term_create(addr);
	wmgr_create(addr);
}
void helper_delete()
{
}