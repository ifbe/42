#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
#define qx q[0]
#define qy q[1]
#define qz q[2]
#define qw q[3]




int qu2eu_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int qu2eu_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	vec4 e;
	float* q;
	struct element* ele;

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	q = buf;
	e[0] = arctan2(2*(qw*qx+qy*qz),1-2*(qx*qx+qy*qy))*180/3.141592653;
	e[1] = arcsin(2*qw*qy - 2*qx*qz)*180/3.141592653;
	e[2] = arctan2(2*(qw*qz+qx*qy),1-2*(qy*qy+qz*qz))*180/3.141592653;

	return relationwrite(ele, _dst_, 0, 0, e, 3);
}
int qu2eu_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int qu2eu_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int qu2eu_create(struct element* ele, u8* url)
{
	say("@qu2eu_create\n");
	return 1;
}
