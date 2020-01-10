#include "struct.h"
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//
#define _ent_ hex32('e','n','t',0)
#define _sup_ hex32('s','u','p',0)
#define _art_ hex32('a','r','t',0)
#define _sys_ hex32('s','y','s',0)
#define _dri_ hex32('d','r','i',0)
#define _dev_ hex32('d','e','v',0)
#define _wrk_ hex32('w','r','k',0)
#define _pwr_ hex32('p','w','r',0)
//bare metal
#define _start_ hex64('s','t','a','r','t', 0, 0, 0)
#define _efimain_ hex64('e','f','i','m','a','i','n', 0)
//module
#define _lib42_ hex64('l','i','b','4','2', 0, 0, 0)
#define _kext_ hex32('k','e','x','t')
#define _ko_ hex32('k','o', 0, 0)
//application
#define _main_ hex32('m','a','i','n')
#define _ndkmain_ hex64('n','d','k','m','a','i','n', 0)
#define _win32_ hex64('w','i','n','3','2', 0, 0, 0)
#define _winmain_ hex64('w','i','n','m','a','i','n', 0)
//
#define _exiter_ hex64('e','x','i','t','e','r', 0, 0)
#define _poller_ hex64('p','o','l','l','e','r', 0, 0)
#define _pulser_ hex64('p','u','l','s','e','r', 0, 0)
#define _realer_ hex64('r','e','a','l','e','r', 0, 0)
#define _waiter_ hex64('w','a','i','t','e','r', 0, 0)




//
int entityread(  struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int entitywrite( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int entitydiscon(struct halfrel* self,struct halfrel* peer);
int entitylinkup(struct halfrel* self,struct halfrel* peer);
int entitydelete(void*);
void* entitycreate(u64 type, void* addr, int argc, u8** argv);
//
int supplyread(  struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int supplywrite( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int supplydiscon(struct halfrel* self,struct halfrel* peer);
int supplylinkup(struct halfrel* self,struct halfrel* peer);
int supplydelete(void*);
void* supplycreate(u64 type, void* addr, int argc, u8** argv);
//
int arteryread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arterywrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int arterydiscon(struct halfrel* self,struct halfrel* peer);
int arterylinkup(struct halfrel* self,struct halfrel* peer);
int arterydelete(void*);
void* arterycreate(u64 type, void* addr, int argc, u8** argv);
//
int systemread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int systemwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int systemdiscon(struct halfrel* self,struct halfrel* peer);
int systemlinkup(struct halfrel* self,struct halfrel* peer);
int systemdelete(void*);
void* systemcreate(u64 type, void* addr, int argc, u8** argv);
//
int driverread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int driverwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int driverdiscon(struct halfrel* self,struct halfrel* peer);
int driverlinkup(struct halfrel* self,struct halfrel* peer);
int driverdelete(void*);
void* drivercreate(u64 type, void* addr, int argc, u8** argv);
//
int deviceread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int devicewrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int devicediscon(struct halfrel* self,struct halfrel* peer);
int devicelinkup(struct halfrel* self,struct halfrel* peer);
int devicedelete(void*);
void* devicecreate(u64 type, void* addr, int argc, u8** argv);
//
int workerread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int workerwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int workerdiscon(struct halfrel* self,struct halfrel* peer);
int workerlinkup(struct halfrel* self,struct halfrel* peer);
int workerdelete(void*);
void* workercreate(u64 type, void* addr, int argc, u8** argv);
//
int pwrclkread( struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int pwrclkwrite(struct halfrel* self,struct halfrel* peer,void* arg,int idx,void* buf,int len);
int pwrclkdiscon(struct halfrel* self,struct halfrel* peer);
int pwrclklinkup(struct halfrel* self,struct halfrel* peer);
int pwrclkdelete(void*);
void* pwrclkcreate(u64 type, void* addr, int argc, u8** argv);
//
int relationread( void* item, int foot, void* arg, int idx, void* buf, int len);
int relationwrite(void* item, int foot, void* arg, int idx, void* buf, int len);
int relationdiscon(struct halfrel* self,struct halfrel* peer);
int relationlinkup(struct halfrel* self,struct halfrel* peer);
int relationdelete(struct relation* rel);
void* relationcreate(void*,void*,u32,u32,void*,void*,u32,u32);
//
int memorydelete(void*);
void* memorycreate(int size, int flag);
int threaddelete(void*);
void* threadcreate(void*, void*);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
int printmemory(void*, int);
int say(void*, ...);
