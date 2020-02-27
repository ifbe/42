#include "libsoft.h"




int tls1v3client_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int tls1v3client_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	say("@tls1v3client_write\n");
	return 0;
}
int tls1v3client_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tls1v3client_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@tls1v3client_linkup\n");
	return 0;
}
int tls1v3client_delete(struct artery* ele)
{
	return 0;
}
int tls1v3client_create(struct artery* ele, u8* url)
{
	say("@tls1v3client_create\n");
	return 0;
}




int tls1v3server_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int tls1v3server_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	say("@tls1v3server_write\n");
	return 0;
}
int tls1v3server_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tls1v3server_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tls1v3server_delete(struct artery* ele)
{
	return 0;
}
int tls1v3server_create(struct artery* ele, u8* url)
{
	return 0;
}




int tls1v3master_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int tls1v3master_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	say("@tls1v3master_write\n");
	printmemory(buf,len);
	return 0;
}
int tls1v3master_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tls1v3master_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@tls1v3master_linkup\n");
	return 0;
}
int tls1v3master_delete(struct artery* ele)
{
	return 0;
}
int tls1v3master_create(struct artery* ele, u8* url)
{
	say("@tls1v3master_create\n");
	return 0;
}