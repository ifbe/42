#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
#define qx q[0]
#define qy q[1]
#define qz q[2]
#define qw q[3]




int qu2eu_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int qu2eu_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@qu2eu_write: foot=%.4s, len=0x%x\n", &foot, len);

	float* q = buf;
	say("	ii: %f,%f,%f,%f\n", q[0],q[1],q[2],q[3]);

	vec4 e;
	e[0] = arctanyx(2*(qw*qx+qy*qz),1-2*(qx*qx+qy*qy))*180/3.141592653;
	e[1] = arcsin(2*qw*qy - 2*qx*qz)*180/3.141592653;
	e[2] = arctanyx(2*(qw*qz+qx*qy),1-2*(qy*qy+qz*qz))*180/3.141592653;
	say("	oo: %f,%f,%f\n",e[0],e[1],e[2]);

	return give_data_into_peer(art,_dst_, stack,sp, 0,0, e,3);
}
int qu2eu_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int qu2eu_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int qu2eu_create(struct artery* ele, u8* url)
{
	say("@qu2eu_create\n");
	return 1;
}
