void asset_create(void*);
void content_create(void*);
void login_create(void*);
void wmgr_create(void*);




void lib4d_create(void* addr)
{
	asset_create(addr);
	content_create(addr);
	login_create(addr);
	wmgr_create(addr);
}
void lib4d_delete()
{
}
