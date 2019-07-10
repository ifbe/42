#include "libuser.h"
#define halfT 0.001
#define Kp 100.0f
#define Ki 0.005f

//????
static float integralx;
static float integraly;
static float integralz;

//quaternion
static float q0;
static float q1;
static float q2;
static float q3;




//accel + gyro
void imuupdate(
	float ax, float ay, float az,
	float gx, float gy, float gz)
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;


	//normalize a
	norm = squareroot(ax*ax+ay*ay+az*az);
	ax = ax/norm;
	ay = ay/norm;
	az = az/norm;


	//
	vx = 2*(q1*q3 - q0*q2);
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

	ex = (ay*vz - az*vy);
	ey = (az*vx - ax*vz);
	ez = (ax*vy - ay*vx);

	integralx += ex*Ki;
	integraly += ey*Ki;
	integralz += ez*Ki;

	gx = gx + Kp*ex + integralx;
	gy = gy + Kp*ey + integraly;
	gz = gz + Kp*ez + integralz;

	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

	norm = squareroot(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;

	//eulerian[0] = arctan2(2*(q0*q1+q2*q3),1-2*(q1*q1+q2*q2))*180/3.141592653;
	//eulerian[1] = arctan2(2*(q0*q3+q1*q2),1-2*(q2*q2+q3*q3))*180/3.141592653;
	//eulerian[2] = arcsin(2*q0*q2 - 2*q1*q3)*180/3.141592653;
	//say("imu:	%lf	%lf	%lf\n", eulerian[0], eulerian[1], eulerian[2]);
}




void easyag_delete()
{
}
void easyag_create()
{
	q0=1.0;
	q1=q2=q3=0;

	integralx=integraly=integralz=0;
}
