#include "naming.h"
#include "struct.h"
#ifndef _OPERATE_H
#define _OPERATE_H




#ifdef __cplusplus
extern "C" {
#endif

int entity_take(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int entity_give(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int entitydiscon(struct halfrel* self, struct halfrel* peer);
int entitylinkup(struct halfrel* self, struct halfrel* peer);
int entitydelete(void*);
void* entitycreate(u64 type, void* addr, int argc, u8** argv);
//
int supply_take(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int supply_give(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int supplydiscon(struct halfrel* self, struct halfrel* peer);
int supplylinkup(struct halfrel* self, struct halfrel* peer);
int supplydelete(struct supply*);
void* supplycreate(u64 type, void* addr, int argc, u8** argv);
//
int artery_take(_art* art,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int artery_give(_art* art,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int arterydiscon(struct halfrel* self, struct halfrel* peer);
int arterylinkup(struct halfrel* self, struct halfrel* peer);
int arterydelete(void*);
void* arterycreate(u64 type, void* addr, int argc, u8** argv);
//
int system_take(_sys* obj,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int system_give(_sys* obj,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int systemdiscon(struct halfrel* self, struct halfrel* peer);
int systemlinkup(struct halfrel* self, struct halfrel* peer);
int systemdelete(void*);
void* systemcreate(u64 type, void* addr, int argc, u8** argv);
//
int driver_take(struct item* dri,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int driver_give(struct item* dri,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int driverdiscon(struct halfrel* self, struct halfrel* peer);
int driverlinkup(struct halfrel* self, struct halfrel* peer);
int driverdelete(void*);
void* drivercreate(u64 type, void* addr, int argc, u8** argv);
//
int device_take(struct item* dev,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int device_give(struct item* dev,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int devicediscon(struct halfrel* self, struct halfrel* peer);
int devicelinkup(struct halfrel* self, struct halfrel* peer);
int devicedelete(void*);
void* devicecreate(u64 type, void* addr, int argc, u8** argv);
//
int bootup_take(struct item* wrk,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int bootup_give(struct item* wrk,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int bootupdiscon(struct halfrel* self, struct halfrel* peer);
int bootuplinkup(struct halfrel* self, struct halfrel* peer);
int bootupdelete(void*);
void* bootupcreate(u64 type, void* addr, int argc, u8** argv);
//
int origin_take(struct item* ori,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int origin_give(struct item* ori,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int origindiscon(struct halfrel* self, struct halfrel* peer);
int originlinkup(struct halfrel* self, struct halfrel* peer);
int origindelete(void*);
void* origincreate(u64 type, void* addr, int argc, u8** argv);
//
int relationdiscon(struct halfrel* self, struct halfrel* peer);
int relationlinkup(struct halfrel* self, struct halfrel* peer);
int relationsearch(void* item,u32 foot, struct halfrel** self,struct halfrel** peer);
int relationmodify(void* item,u32 foot, struct halfrel** self,struct halfrel** peer);
int relationdelete(_rel* rel);
void* relationcreate(void*,void*,u32,u32,void*,void*,u32,u32);
//
int take_data_from_peer(void* chip,int ftype, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int give_data_into_peer(void* chip,int ftype, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int take_data_from_them(void* item,int ftype, _syn* stack,int sp, void* arg,int key, void* buf,int len);
int give_data_into_them(void* item,int ftype, _syn* stack,int sp, void* arg,int key, void* buf,int len);

#ifdef __cplusplus
}
#endif
#endif
