#include "naming.h"
#include "struct.h"
#ifndef _OPERATE_H
#define _OPERATE_H




#ifdef __cplusplus
extern "C" {
#endif

int entityread( _ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int entitywrite(_ent* ent,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int entitydiscon(struct halfrel* self, struct halfrel* peer);
int entitylinkup(struct halfrel* self, struct halfrel* peer);
int entitydelete(void*);
void* entitycreate(u64 type, void* addr, int argc, u8** argv);
//
int supplyread( _sup* sup,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int supplywrite(_sup* sup,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int supplydiscon(struct halfrel* self, struct halfrel* peer);
int supplylinkup(struct halfrel* self, struct halfrel* peer);
int supplydelete(struct supply*);
void* supplycreate(u64 type, void* addr, int argc, u8** argv);
//
int arteryread( _art* art,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int arterywrite(_art* art,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int arterydiscon(struct halfrel* self, struct halfrel* peer);
int arterylinkup(struct halfrel* self, struct halfrel* peer);
int arterydelete(void*);
void* arterycreate(u64 type, void* addr, int argc, u8** argv);
//
int systemread( _sys* obj,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int systemwrite(_sys* obj,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int systemdiscon(struct halfrel* self, struct halfrel* peer);
int systemlinkup(struct halfrel* self, struct halfrel* peer);
int systemdelete(void*);
void* systemcreate(u64 type, void* addr, int argc, u8** argv);
//
int driverread( struct item* dri,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int driverwrite(struct item* dri,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int driverdiscon(struct halfrel* self, struct halfrel* peer);
int driverlinkup(struct halfrel* self, struct halfrel* peer);
int driverdelete(void*);
void* drivercreate(u64 type, void* addr, int argc, u8** argv);
//
int deviceread( struct item* dev,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int devicewrite(struct item* dev,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int devicediscon(struct halfrel* self, struct halfrel* peer);
int devicelinkup(struct halfrel* self, struct halfrel* peer);
int devicedelete(void*);
void* devicecreate(u64 type, void* addr, int argc, u8** argv);
//
int workerread( struct item* wrk,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int workerwrite(struct item* wrk,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int workerdiscon(struct halfrel* self, struct halfrel* peer);
int workerlinkup(struct halfrel* self, struct halfrel* peer);
int workerdelete(void*);
void* workercreate(u64 type, void* addr, int argc, u8** argv);
//
int originread( struct item* ori,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int originwrite(struct item* ori,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int origindiscon(struct halfrel* self, struct halfrel* peer);
int originlinkup(struct halfrel* self, struct halfrel* peer);
int origindelete(void*);
void* origincreate(u64 type, void* addr, int argc, u8** argv);
//
int take_data_from_peer(void* item,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int give_data_into_peer(void* item,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int relationdiscon(struct halfrel* self, struct halfrel* peer);
int relationlinkup(struct halfrel* self, struct halfrel* peer);
int relationsearch(void* item,u32 foot, struct halfrel** self,struct halfrel** peer);
int relationmodify(void* item,u32 foot, struct halfrel** self,struct halfrel** peer);
int relationdelete(_rel* rel);
void* relationcreate(void*,void*,u32,u32,void*,void*,u32,u32);

#ifdef __cplusplus
}
#endif
#endif
