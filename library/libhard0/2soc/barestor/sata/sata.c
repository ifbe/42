#include "libhard.h"
void filemanager_registerdisk(void*,void*);




static int sata_takeby(struct item* sata,void* selfrel, void* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
    struct halfrel* peer = sata->priv_ptr;
    struct item* ahci = peer ? peer->pchip : 0;
    struct item* port = peer ? peer->pfoot : 0;
    logtoall("%s: cmd=%x, peer=%p, ahci=%p\n", __FUNCTION__, cmd, peer, ahci);

    if(0 == ahci)return -1;
    if(0 == port)return -2;
    return ahci->ontaking(ahci, port, stack, sp, arg, cmd, buf, len);
}
static int sata_giveby(struct item* sata,void* selfrel, void* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
    return 0;
}
static int sata_detach(struct halfrel* self, struct halfrel* peer)
{
    return 0;
}
static int sata_attach(struct halfrel* self, struct halfrel* peer)
{
    struct item* sata = self->pchip;
    struct item* that = peer->pchip;
    logtoall("%s: cmd=%x\n", __FUNCTION__);

    if(that->type == _ahci_){
        sata->priv_ptr = peer;
	    filemanager_registerdisk(sata,0);
    }
    return 0;
}


static int sata_reader(struct item* sata,void* selfrel, p64 arg,int cmd, void* buf,int len)
{
    //logtoall("%s: cmd=%x\n", __FUNCTION__, cmd);
    struct halfrel* peer = sata->priv_ptr;
    struct item* ahci = peer ? peer->pchip : 0;
    struct item* port = peer ? peer->pfoot : 0;
    logtoall("%s: cmd=%x, peer=%p, ahci=%p, port=%p\n", __FUNCTION__, cmd, peer, ahci, port);

    if(0 == ahci)return -1;
    if(0 == port)return -2;
    if(0 == ahci->onreader)return -3;
    return ahci->onreader(ahci, port, arg, cmd, buf, len);
}
static int sata_writer(struct item* sata,void* selfrel, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
static int sata_delete(struct item* sata)
{
    return 0;
}
static int sata_create(struct item* sata)
{
    logtoall("%s\n", __FUNCTION__);
    return 0;
}


void* sata_alloc()
{
	struct item* sata = device_alloc_fromtype(_sata_);
	sata->tier = _dev_;
	sata->kind = _stor_;
    //sata->type = _sata_;

    sata->oncreate = (void*)sata_create;
    sata->ondelete = (void*)sata_delete;
    sata->onattach = (void*)sata_attach;
    sata->ondetach = (void*)sata_detach;
    sata->onreader = (void*)sata_reader;
    sata->onwriter = (void*)sata_writer;
    sata->ontaking = (void*)sata_takeby;
    sata->ongiving = (void*)sata_giveby;
    return sata;
}