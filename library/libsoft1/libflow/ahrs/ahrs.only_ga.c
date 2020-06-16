#include "libuser.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)

#define halfT 0.001
#define Kp 100.0f
#define Ki 0.005f

//????
static float integralx;
static float integraly;
static float integralz;

//quaternion
static float q[4];
#define qx q[0]
#define qy q[1]
#define qz q[2]
#define qw q[3]




//accel + gyro
void imuupdate(
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

	integralx += ex*Ki;
	integraly += ey*Ki;
	integralz += ez*Ki;

	gx = gx + Kp*ex + integralx;
	gy = gy + Kp*ey + integraly;
	gz = gz + Kp*ez + integralz;

	qw = qw + (-qx*gx - qy*gy - qz*gz)*halfT;
	qx = qx + (qw*gx + qy*gz - qz*gy)*halfT;
	qy = qy + (qw*gy - qx*gz + qz*gx)*halfT;
	qz = qz + (qw*gz + qx*gy - qy*gx)*halfT;

	norm = squareroot(qw*qw + qx*qx + qy*qy + qz*qz);
	qw = qw / norm;
	qx = qx / norm;
	qy = qy / norm;
	qz = qz / norm;

	//vx = arctan2(2*(qw*qx+qy*qz),1-2*(qx*qx+qy*qy))*180/3.141592653;
	//vy = arcsin(2*qw*qy - 2*qx*qz)*180/3.141592653;
	//vz = arctan2(2*(qw*qz+qx*qy),1-2*(qy*qy+qz*qz))*180/3.141592653;
	//say("euler:	%f	%f	%f\n", vx, vy, vz);
}




int easyag_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	float f[10];
	say("@easyag_read\n");

	take_data_from_peer(art,_src_, stack,sp, 0,0, f,10);
	return 0;
}
int easyag_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@easyag_write: foot=%.4s, len=0x%x\n", &foot, len);
	if((6 != len) && (9 != len)){
		say("err@easyag_write:len=%d\n",len);
		return 0;
	}

	float* f = (void*)buf;
	say("	ii: %f,%f,%f,%f,%f,%f\n",f[0],f[1],f[2], f[3],f[4],f[5]);

	imuupdate(f[0],f[1],f[2], f[3],f[4],f[5]);
	say("	oo: %f,%f,%f,%f\n",q[0],q[1],q[2],q[3]);

	give_data_into_peer(art,_dst_, stack,sp, 0,0, q,4);
	return 0;
}
int easyag_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@easyag_discon\n");
	return 0;
}
int easyag_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@easyag_linkup\n");
	return 0;
}




void easyag_delete(struct artery* ele)
{
}
void easyag_create(struct artery* ele, u8* url)
{
	qw = 1.0;
	qx = qy = qz = 0;

	integralx = integraly = integralz = 0;
}
