#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
double squareroot(double);
//
#define tenms 0.01
//
#define twoKp 2.0
#define twoKi 0.05
//
static float q[4];
#define qx q[0]
#define qy q[1]
#define qz q[2]
#define qw q[3]
//
static float integralx, integraly, integralz;



/*
origin code: https://github.com/PaulStoffregen/MahonyAHRS
axis system: east-north-sky, right-front-top
input ax/ay/az: accel direction and strength, unit=m*(s^2), when lay flat value=(0,0,-9.8)
input gx/gy/gz: speed of rotate around x/y/z axis, unit=rad/s, right hand law, value>0

why cross(measure, pridict), not cross(predict, measure)?
1.think bodyspace as a cube, a human at cube center
2.at t0: predict attitude=0, so predict gravity vector=vertically downward
  measure angular speed=0, measure gravity vector=center to right foot
3.at t1: align right leg vector with gravity vector,
  so predict attitude=rotate around front vector=cross(measure_gravity, predict_gravity)
*/




static float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	y = y * (1.5f - (halfx * y * y));
	return y;
}
void mahonyupdate3(
	float gx, float gy, float gz,
	float deltaT)
{
	gx *= (0.5f * deltaT);
	gy *= (0.5f * deltaT);
	gz *= (0.5f * deltaT);

	float pw = qw;
	float px = qx;
	float py = qy;
	float pz = qz;
	qx += (  0 * px +gz * py -gy * pz +gx * pw);
	qy += (-gz * px + 0 * py +gx * pz +gy * pw);
	qz += ( gy * px -gx * py + 0 * px +gz * pw);
	qw += (-gx * px -gy * py -gz * pz + 0 * pw);

	float invnorm = invSqrt(qw * qw + qx * qx + qy * qy + qz * qz);
	qw *= invnorm;
	qx *= invnorm;
	qy *= invnorm;
	qz *= invnorm;
}
void mahonyupdate6(
	float gx, float gy, float gz,
	float ax, float ay, float az,
	float deltaT)
{
	if( (ax == 0.0f) && (ay == 0.0f) && (az == 0.0f) ){
		mahonyupdate3(gx, gy, gz, deltaT);
		return;
	}

	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;

	// bodyspace measure_grav_dir
	recipNorm = invSqrt(ax * ax + ay * ay + az * az);
	ax *= recipNorm;
	ay *= recipNorm;
	az *= recipNorm;

	// bodyspace predict_grav_dir
	halfvx = -(qx * qz - qw * qy);
	halfvy = -(qw * qx + qy * qz);
	halfvz = -(qw * qw + qz * qz - 0.5);

	// bodyspace grav_error = cross(measure_grav_dir, predict_grav_dir)
	halfex = (ay * halfvz - az * halfvy);
	halfey = (az * halfvx - ax * halfvz);
	halfez = (ax * halfvy - ay * halfvx);

	// Compute and apply integral feedback if enabled
	if(twoKi > 0.0f) {
		// integral error scaled by Ki
		integralx += twoKi * halfex * deltaT;
		integraly += twoKi * halfey * deltaT;
		integralz += twoKi * halfez * deltaT;
		gx += integralx;	// apply integral feedback
		gy += integraly;
		gz += integralz;
	}
	else {
		integralx = 0.0f;	// prevent integral windup
		integraly = 0.0f;
		integralz = 0.0f;
	}

	// Apply proportional feedback
	gx += twoKp * halfex;
	gy += twoKp * halfey;
	gz += twoKp * halfez;

	//use new gyro val
	mahonyupdate3(gx, gy, gz, deltaT);
}
void mahonyupdate9(
	float gx, float gy, float gz,
	float ax, float ay, float az,
	float mx, float my, float mz,
	float deltaT)
{
	if( (mx == 0.0f) && (my == 0.0f) && (mz == 0.0f) ){
		mahonyupdate6(gx, gy, gz, ax, ay, az, deltaT);
		return;
	}

	float norm;
	float qwqw, qwqx, qwqy, qwqz, qxqx, qxqy, qxqz, qyqy, qyqz, qzqz;  
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;

	// bodyspace measure_grav_dir
	norm = squareroot(ax * ax + ay * ay + az * az);
	ax /= norm;
	ay /= norm;
	az /= norm;     

	// bodyspace measure_mag_dir
	norm = squareroot(mx * mx + my * my + mz * mz);
	mx /= norm;
	my /= norm;
	mz /= norm;   

	// Auxiliary variables to avoid repeated arithmetic
	qwqw = qw * qw;
	qwqx = qw * qx;
	qwqy = qw * qy;
	qwqz = qw * qz;
	qxqx = qx * qx;
	qxqy = qx * qy;
	qxqz = qx * qz;
	qyqy = qy * qy;
	qyqz = qy * qz;
	qzqz = qz * qz;

	// bodyspace predict_grav_dir
	halfvx = -(qxqz - qwqy);
	halfvy = -(qwqx + qyqz);
	halfvz = -(qwqw + qzqz - 0.5);

	// bodyspace grav_error = cross(measure_grav_dir, predict_grav_dir)
	halfex = (ay * halfvz - az * halfvy);
	halfey = (az * halfvx - ax * halfvz);
	halfez = (ax * halfvy - ay * halfvx);

	// bodyspace predict_mag_dir
	hx = 2.0f * (mx * (0.5f - qyqy - qzqz) + my * (qxqy - qwqz) + mz * (qxqz + qwqy));
	hy = 2.0f * (mx * (qxqy + qwqz) + my * (0.5f - qxqx - qzqz) + mz * (qyqz - qwqx));
	bx = squareroot(hx * hx + hy * hy);
	bz = 2.0f * (mx * (qxqz - qwqy) + my * (qyqz + qwqx) + mz * (0.5f - qxqx - qyqy));
	halfwx = bx * (0.5f - qyqy - qzqz) + bz * (qxqz - qwqy);
	halfwy = bx * (qxqy - qwqz) + bz * (qwqx + qyqz);
	halfwz = bx * (qwqy + qxqz) + bz * (0.5f - qxqx - qyqy);  

	// bodyspace mag_error = cross(measure_mag_dir, predict_mag_dir)
	halfex += my * halfwz - mz * halfwy;
	halfey += mz * halfwx - mx * halfwz;
	halfez += mx * halfwy - my * halfwx;

	// Compute and apply integral feedback if enabled
	if(twoKi > 0.0f){
		// integral error scaled by Ki
		integralx += twoKi * halfex * deltaT;
		integraly += twoKi * halfey * deltaT;
		integralz += twoKi * halfez * deltaT;

		gx += integralx;	// apply integral feedback
		gy += integraly;
		gz += integralz;
	}
	else{
		integralx = 0.0f;	// prevent integral windup
		integraly = 0.0f;
		integralz = 0.0f;
	}

	// Apply proportional feedback
	gx += twoKp * halfex;
	gy += twoKp * halfey;
	gz += twoKp * halfez;

	//use new gyro val
	mahonyupdate3(gx, gy, gz, deltaT);
}




int mahony_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	logtoall("@mahony_read\n");

	float f[10];
	take_data_from_peer(art,_src_, stack,sp, 0,0, f,10);
	return 0;
}
int mahony_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	logtoall("@mahony_write\n");

	float* f = (void*)buf;
	switch(len){
		case 9:mahonyupdate9(f[0],f[1],f[2], f[3],f[4],f[5], f[6],f[7],f[8], tenms);break;
		case 6:mahonyupdate6(f[0],f[1],f[2], f[3],f[4],f[5], tenms);break;
		default:logtoall("err@mahony_write:len=%d\n", len);return 0;
	}

	give_data_into_peer(art,_dst_, stack,sp, 0,0, q,4);
	return 0;
}
int mahony_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@mahony_detach\n");
	return 0;
}
int mahony_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@mahony_attach\n");
	return 0;
}




void mahony_search(_obj* ele)
{
}
void mahony_modify(_obj* ele, int foot, float* f, int cmd, float* buf, int len)
{
	mahonyupdate9(f[0],f[1],f[2], f[3],f[4],f[5], f[6],f[7],f[8], tenms);

	buf[0] = q[0];
	buf[1] = q[1];
	buf[2] = q[2];
	buf[3] = q[3];
}
int mahony_delete(_obj* ele)
{
	return 0;
}
int mahony_create(_obj* ele, u8* arg)
{
	logtoall("@mahony_create\n");

	qw = 1.0;
	qx = qy = qz = 0.0;
	integralx = integraly = integralz = 0.0;
	return 1;
}
