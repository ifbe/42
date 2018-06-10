void asset_create(void*);
void content_create(void*);
void helpin_create(void*);
void helpout_create(void*);
void overview_create(void*);
void posture_create(void*);




void lib4d_create(void* addr)
{
	asset_create(addr);
	content_create(addr);

	helpin_create(addr);
	helpout_create(addr);

	overview_create(addr);
	posture_create(addr);
}
void lib4d_delete()
{
}
