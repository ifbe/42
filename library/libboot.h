#include "naming.h"
#include "struct.h"




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
