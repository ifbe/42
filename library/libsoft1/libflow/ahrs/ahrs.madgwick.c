#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
double squareroot(double);

//
#define beta 0.5
#define qx per->q[0]
#define qy per->q[1]
#define qz per->q[2]
#define qw per->q[3]
#define gx pg[0]
#define gy pg[1]
#define gz pg[2]
#define ax pa[0]
#define ay pa[1]
#define az pa[2]
#define mx pm[0]
#define my pm[1]
#define mz pm[2]
//
struct perimu{
	float q[4];
	float integralx, integraly, integralz;
	float dt;
};




//https://github.com/PaulStoffregen/MadgwickAHRS
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
void madgwickupdate6(struct perimu* per, float* pg, float* pa)
{
	float recipNorm;
	float s0, s1, s2, s3;
	float qwqw, qxqx, qyqy, qzqz;
	float qDot1, qDot2, qDot3, qDot4;
	float _2qw, _2qx, _2qy, _2qz, _4qw, _4qx, _4qy ,_8qx, _8qy;


	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-qx * gx - qy * gy - qz * gz);
	qDot2 = 0.5f * (qw * gx + qy * gz - qz * gy);
	qDot3 = 0.5f * (qw * gy - qx * gz + qz * gx);
	qDot4 = 0.5f * (qw * gz + qx * gy - qy * gx);


	// Normalise accelerometer measurement
	recipNorm = invSqrt(ax * ax + ay * ay + az * az);
	ax *= recipNorm;
	ay *= recipNorm;
	az *= recipNorm;


	// Auxiliary variables to avoid repeated arithmetic
	_2qw = 2.0f * qw;
	_2qx = 2.0f * qx;
	_2qy = 2.0f * qy;
	_2qz = 2.0f * qz;
	_4qw = 4.0f * qw;
	_4qx = 4.0f * qx;
	_4qy = 4.0f * qy;
	_8qx = 8.0f * qx;
	_8qy = 8.0f * qy;
	qwqw = qw * qw;
	qxqx = qx * qx;
	qyqy = qy * qy;
	qzqz = qz * qz;


	// Gradient decent algorithm corrective step
	s0 = _4qw*qyqy + _2qy*ax + _4qw*qxqx - _2qx*ay;
	s1 = _4qx*qzqz - _2qz*ax + 4.0*qwqw*qx - _2qw*ay - _4qx + _8qx*qxqx + _8qx*qyqy + _4qx*az;
	s2 = 4.0f * qwqw*qy + _2qw*ax + _4qy*qzqz - _2qz*ay - _4qy + _8qy*qxqx + _8qy*qyqy + _4qy*az;
	s3 = 4.0f * qxqx*qz - _2qx*ax + 4.0*qyqy * qz - _2qy*ay;
	recipNorm = invSqrt(s0*s0 + s1*s1 + s2*s2 + s3*s3); // normalise step magnitude
	s0 *= recipNorm;
	s1 *= recipNorm;
	s2 *= recipNorm;
	s3 *= recipNorm;


	// Apply feedback step
	qDot1 -= beta * s0;
	qDot2 -= beta * s1;
	qDot3 -= beta * s2;
	qDot4 -= beta * s3;


	// Integrate rate of change of quaternion to yield quaternion
	qw += qDot1 * per->dt;
	qx += qDot2 * per->dt;
	qy += qDot3 * per->dt;
	qz += qDot4 * per->dt;


	// Normalise quaternion
	recipNorm = invSqrt(qw * qw + qx * qx + qy * qy + qz * qz);
	qw *= recipNorm;
	qx *= recipNorm;
	qy *= recipNorm;
	qz *= recipNorm;
}
void madgwickupdate9(struct perimu* per, float* pg, float* pa, float* pm)
{
	float norm;
	float hx, hy;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2qwmx, _2qwmy, _2qwmz, _2qxmx;
	float _2bx, _2bz, _4bx, _4bz;
	float _2qw, _2qx, _2qy, _2qz;
	float _2qwqy, _2qyqz;
	float qwqw, qwqx, qwqy, qwqz, qxqx, qxqy, qxqz, qyqy, qyqz, qzqz;


	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-qx * gx - qy * gy - qz * gz);
	qDot2 = 0.5f * (qw * gx + qy * gz - qz * gy);
	qDot3 = 0.5f * (qw * gy - qx * gz + qz * gx);
	qDot4 = 0.5f * (qw * gz + qx * gy - qy * gx);


	// Normalise accelerometer measurement
	norm = -squareroot(ax * ax + ay * ay + az * az);
	ax /= norm;
	ay /= norm;
	az /= norm;   


	// Normalise magnetometer measurement
	norm = squareroot(mx * mx + my * my + mz * mz);
	mx /= norm;
	my /= norm;
	mz /= norm;


	// Auxiliary variables to avoid repeated arithmetic
	_2qwmx = 2.0f * qw * mx;
	_2qwmy = 2.0f * qw * my;
	_2qwmz = 2.0f * qw * mz;
	_2qxmx = 2.0f * qx * mx;

	_2qw = 2.0f * qw;
	_2qx = 2.0f * qx;
	_2qy = 2.0f * qy;
	_2qz = 2.0f * qz;

	_2qwqy = 2.0f * qw * qy;
	_2qyqz = 2.0f * qy * qz;

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


	// Reference direction of Earth's magnetic field
	hx = mx*qwqw - _2qwmy*qz + _2qwmz*qy + mx*qxqx + _2qx*my*qy + _2qx*mz*qz - mx*qyqy- mx*qzqz;
	hy = _2qwmx*qz + my*qwqw - _2qwmz*qx + _2qxmx*qy - my*qxqx + my*qyqy + _2qy*mz*qz - my*qzqz;

	_2bx = squareroot(hx * hx + hy * hy);
	_2bz = -_2qwmx*qy + _2qwmy*qx + mz*qwqw + _2qxmx*qz - mz*qxqx + _2qy*my*qz - mz*qyqy + mz*qzqz;

	_4bx	= 2.0f * _2bx;
	_4bz	= 2.0f * _2bz;


	// Gradient decent algorithm corrective step
	s0	= -_2qy * (2.0f * qxqz - _2qwqy - ax)
		+ _2qx * (2.0f * qwqx + _2qyqz - ay)
		- _2bz * qy * (_2bx * (0.5f - qyqy - qzqz) + _2bz * (qxqz - qwqy) - mx)
		+ (-_2bx * qz + _2bz * qx) * (_2bx * (qxqy - qwqz) + _2bz * (qwqx + qyqz) - my)
		+ _2bx * qy * (_2bx * (qwqy + qxqz) + _2bz * (0.5f - qxqx - qyqy) - mz);

	s1	= _2qz * (2.0f * qxqz - _2qwqy - ax)
		+ _2qw * (2.0f * qwqx + _2qyqz - ay)
		- 4.0f * qx * (1 - 2.0f * qxqx - 2.0f * qyqy - az)
		+ _2bz * qz * (_2bx * (0.5f - qyqy - qzqz) + _2bz * (qxqz - qwqy) - mx)
		+ (_2bx * qy + _2bz * qw) * (_2bx * (qxqy - qwqz) + _2bz * (qwqx + qyqz) - my)
		+ (_2bx * qz - _4bz * qx) * (_2bx * (qwqy + qxqz) + _2bz * (0.5f - qxqx - qyqy) - mz);

	s2	= -_2qw * (2.0f * qxqz - _2qwqy - ax)
		+ _2qz * (2.0f * qwqx + _2qyqz - ay)
		- 4.0f * qy * (1 - 2.0f * qxqx - 2.0f * qyqy - az)
		+ (-_4bx * qy - _2bz * qw) * (_2bx * (0.5f - qyqy - qzqz) + _2bz * (qxqz - qwqy) - mx)
		+ (_2bx * qx + _2bz * qz) * (_2bx * (qxqy - qwqz) + _2bz * (qwqx + qyqz) - my)
		+ (_2bx * qw - _4bz * qy) * (_2bx * (qwqy + qxqz) + _2bz * (0.5f - qxqx - qyqy) - mz);

	s3	= _2qx * (2.0f * qxqz - _2qwqy - ax)
		+ _2qy * (2.0f * qwqx + _2qyqz - ay)
		+ (-_4bx * qz + _2bz * qx) * (_2bx * (0.5f - qyqy - qzqz) + _2bz * (qxqz - qwqy) - mx)
		+ (-_2bx * qw + _2bz * qy) * (_2bx * (qxqy - qwqz) + _2bz * (qwqx + qyqz) - my)
		+ _2bx * qx * (_2bx * (qwqy + qxqz) + _2bz * (0.5f - qxqx - qyqy) - mz);


	// normalise step magnitude	// Apply feedback step
	norm = squareroot(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
	qDot1 -= beta * s0 / norm;
	qDot2 -= beta * s1 / norm;
	qDot3 -= beta * s2 / norm;
	qDot4 -= beta * s3 / norm;


	// Integrate rate of change of quaternion to yield quaternion
	qw += qDot1 * per->dt;
	qx += qDot2 * per->dt;
	qy += qDot3 * per->dt;
	qz += qDot4 * per->dt;


	// Normalise quaternion
	norm = squareroot(qw * qw + qx * qx + qy * qy + qz * qz);
	qw /= norm;
	qx /= norm;
	qy /= norm;
	qz /= norm;


	//gx = arctanyx(2*(qw*qx+qy*qz),1-2*(qx*qx+qy*qy))*180/3.141592653;
	//gy = arcsin(2*qw*qy - 2*qx*qz)*180/3.141592653;
	//gz = arctanyx(2*(qw*qz+qx*qy),1-2*(qy*qy+qz*qz))*180/3.141592653;
	//say("euler:	%f	%f	%f\n", gx, gy, gz);
}




int madgwick_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@madgwick_read\n");

	float f[10];
	take_data_from_peer(art,_src_, stack,sp, 0,0, f,10);
	return 0;
}
int madgwick_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@madgwick_write\n");

	struct perimu* per = (void*)art->priv_256b;
	float* f = (void*)buf;
	switch(len){
		case 9:madgwickupdate9(per, &f[0], &f[3], &f[6]);break;
		case 6:madgwickupdate6(per, &f[0], &f[3]);break;
		default:say("err@madgwick_write:len=%d\n", len);return 0;
	}

	give_data_into_peer(art,_dst_, stack,sp, 0,0, per->q,4);
	return 0;
}
int madgwick_detach(struct halfrel* self, struct halfrel* peer)
{
	say("@madgwick_detach\n");
	return 0;
}
int madgwick_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@madgwick_attach\n");
	return 0;
}




void madgwick_search(_obj* ele)
{
}
void madgwick_modify(_obj* ele, int foot, float* f, int cmd, float* buf, int len)
{
	struct perimu* per = (void*)ele->priv_256b;

	madgwickupdate9(per, &f[0], &f[3], &f[6]);

	buf[0] = qx;
	buf[1] = qy;
	buf[2] = qz;
	buf[3] = qw;
}
int madgwick_delete(_obj* ele)
{
	return 0;
}
int madgwick_create(_obj* ele, u8* arg)
{
	say("@madgwick_create\n");

	struct perimu* per = (void*)ele->priv_256b;

	qx = qy = qz = 0.0;
	qw = 1.0;

	per->integralx = per->integraly = per->integralz = 0.0;

	per->dt = 0.01;
	return 1;
}
