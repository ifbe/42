void content_create(void*);
void login_create(void*);
void wmgr_create(void*);




void lib4d_create(void* addr)
{
	content_create(addr);
	login_create(addr);
	wmgr_create(addr);
}
void lib4d_delete()
{
}
