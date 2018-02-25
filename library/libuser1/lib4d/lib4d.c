void helpin_create(void*);
void helpout_create(void*);
void asset_create(void*);
void content_create(void*);
void login_create(void*);




void lib4d_create(void* addr)
{
	helpin_create(addr);
	helpout_create(addr);

	asset_create(addr);
	content_create(addr);
	login_create(addr);
}
void lib4d_delete()
{
}
