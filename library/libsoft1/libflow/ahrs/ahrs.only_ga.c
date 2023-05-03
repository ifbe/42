#include "libuser.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)

#define Kp 100.0f
#define Ki 0.005f
#define qx per->q[0]
#define qy per->q[1]
#define qz per->q[2]
#define qw per->q[3]
struct perimu{
	float q[4];
	float integralx, integraly, integralz;
	float dt;
};




//accel + gyro
void imuupdate(struct perimu* per,
	float gx, float gy, float gz,
	float ax, float ay, float az)
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;

	//normalize a
	norm = squareroot(ax*ax+ay*ay+az*az);
	if(norm < 1e-18)return;
	ax = ax/norm;
	ay = ay/norm;
	az = az/norm;

	//
	vx = 2*(qx*qz - qw*qy);
	vy = 2*(qw*qx + qy*qz);
	vz = qw*qw - qx*qx - qy*qy + qz*qz;

	ex = vy*az - vz*ay;
	ey = vz*ax - vx*az;
	ez = vx*ay - vy*ax;

	per->integralx += ex*Ki;
	per->integraly += ey*Ki;
	per->integralz += ez*Ki;

	gx = gx + Kp*ex + per->integralx;
	gy = gy + Kp*ey + per->integraly;
	gz = gz + Kp*ez + per->integralz;

	qw = qw + (-qx*gx - qy*gy - qz*gz) * per->dt/2.0;
	qx = qx + ( qw*gx + qy*gz - qz*gy) * per->dt/2.0;
	qy = qy + ( qw*gy - qx*gz + qz*gx) * per->dt/2.0;
	qz = qz + ( qw*gz + qx*gy - qy*gx) * per->dt/2.0;

	norm = squareroot(qw*qw + qx*qx + qy*qy + qz*qz);
	qw = qw / norm;
	qx = qx / norm;
	qy = qy / norm;
	qz = qz / norm;

	//vx = arctanyx(2*(qw*qx+qy*qz),1-2*(qx*qx+qy*qy))*180/3.141592653;
	//vy = arcsin(2*qw*qy - 2*qx*qz)*180/3.141592653;
	//vz = arctanyx(2*(qw*qz+qx*qy),1-2*(qy*qy+qz*qz))*180/3.141592653;
	//say("euler:	%f	%f	%f\n", vx, vy, vz);
}




int easyag_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	float f[10];
	say("@easyag_read\n");

	take_data_from_peer(art,_src_, stack,sp, 0,0, f,10);
	return 0;
}
int easyag_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@easyag_write: foot=%.4s, len=0x%x\n", &foot, len);
	if((6 != len) && (9 != len)){
		say("err@easyag_write:len=%d\n",len);
		return 0;
	}

	float* f = (void*)buf;
	say("	ii: %f,%f,%f,%f,%f,%f\n",f[0],f[1],f[2], f[3],f[4],f[5]);

	struct perimu* per = (void*)art->priv_256b;
	imuupdate(per, f[0],f[1],f[2], f[3],f[4],f[5]);
	say("	oo: %f,%f,%f,%f\n",qx,qy,qz,qw);

	give_data_into_peer(art,_dst_, stack,sp, 0,0, per->q,4);
	return 0;
}
int easyag_detach(struct halfrel* self, struct halfrel* peer)
{
	say("@easyag_detach\n");
	return 0;
}
int easyag_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@easyag_attach\n");
	return 0;
}




void easyag_search(_obj* ele)
{
}
void easyag_modify(_obj* ele, int foot, float* f, int cmd, float* buf, int len)
{
	struct perimu* per = (void*)ele->priv_256b;
	imuupdate(per, f[0],f[1],f[2], f[3],f[4],f[5]);

	buf[0] = qx;
	buf[1] = qy;
	buf[2] = qz;
	buf[3] = qw;
}
void easyag_delete(_obj* ele)
{
}
void easyag_create(_obj* ele, u8* arg)
{
	struct perimu* per = (void*)ele->priv_256b;

	qx = qy = qz = 0;
	qw = 1.0;

	per->integralx = per->integraly = per->integralz = 0;
	per->dt = 0.01;
}
