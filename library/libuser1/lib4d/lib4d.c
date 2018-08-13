void asset_create(void*);
void content_create(void*);
void bgfg_create(void*);
void helpout_create(void*);
void overview_create(void*);




void lib4d_create(void* addr)
{
	asset_create(addr);
	bgfg_create(addr);
	content_create(addr);

	overview_create(addr);
}
void lib4d_delete()
{
}
