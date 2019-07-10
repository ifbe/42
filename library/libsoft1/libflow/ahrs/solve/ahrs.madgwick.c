#include "libsoft.h"
double squareroot(double);

//
#define T 0.001
#define beta 0.1

//
float q0, q1, q2, q3;
float integralx, integraly, integralz;




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
void madgwickupdate6(
	float gx, float gy, float gz,
	float ax, float ay, float az)
{
	float recipNorm;
	float s0, s1, s2, s3;
	float q0q0, q1q1, q2q2, q3q3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2;


	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);


	// Normalise accelerometer measurement
	recipNorm = invSqrt(ax * ax + ay * ay + az * az);
	ax *= recipNorm;
	ay *= recipNorm;
	az *= recipNorm;


	// Auxiliary variables to avoid repeated arithmetic
	_2q0 = 2.0f * q0;
	_2q1 = 2.0f * q1;
	_2q2 = 2.0f * q2;
	_2q3 = 2.0f * q3;
	_4q0 = 4.0f * q0;
	_4q1 = 4.0f * q1;
	_4q2 = 4.0f * q2;
	_8q1 = 8.0f * q1;
	_8q2 = 8.0f * q2;
	q0q0 = q0 * q0;
	q1q1 = q1 * q1;
	q2q2 = q2 * q2;
	q3q3 = q3 * q3;


	// Gradient decent algorithm corrective step
	s0 = _4q0*q2q2 + _2q2*ax + _4q0*q1q1 - _2q1*ay;
	s1 = _4q1*q3q3 - _2q3*ax + 4.0*q0q0*q1 - _2q0*ay - _4q1 + _8q1*q1q1 + _8q1*q2q2 + _4q1*az;
	s2 = 4.0f * q0q0*q2 + _2q0*ax + _4q2*q3q3 - _2q3*ay - _4q2 + _8q2*q1q1 + _8q2*q2q2 + _4q2*az;
	s3 = 4.0f * q1q1*q3 - _2q1*ax + 4.0*q2q2 * q3 - _2q2*ay;
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
	q0 += qDot1 * T;
	q1 += qDot2 * T;
	q2 += qDot3 * T;
	q3 += qDot4 * T;


	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}
void madgwickupdate9(
	float ax, float ay, float az,
	float gx, float gy, float gz,
	float mx, float my, float mz)
{
	float norm;
	float hx, hy;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx;
	float _2bx, _2bz, _4bx, _4bz;
	float _2q0, _2q1, _2q2, _2q3;
	float _2q0q2, _2q2q3;
	float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;


	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);


	// Normalise accelerometer measurement
	norm = squareroot(ax * ax + ay * ay + az * az);
	ax /= norm;
	ay /= norm;
	az /= norm;   


	// Normalise magnetometer measurement
	norm = squareroot(mx * mx + my * my + mz * mz);
	mx /= norm;
	my /= norm;
	mz /= norm;


	// Auxiliary variables to avoid repeated arithmetic
	_2q0mx = 2.0f * q0 * mx;
	_2q0my = 2.0f * q0 * my;
	_2q0mz = 2.0f * q0 * mz;
	_2q1mx = 2.0f * q1 * mx;

	_2q0 = 2.0f * q0;
	_2q1 = 2.0f * q1;
	_2q2 = 2.0f * q2;
	_2q3 = 2.0f * q3;

	_2q0q2 = 2.0f * q0 * q2;
	_2q2q3 = 2.0f * q2 * q3;

	q0q0 = q0 * q0;
	q0q1 = q0 * q1;
	q0q2 = q0 * q2;
	q0q3 = q0 * q3;
	q1q1 = q1 * q1;
	q1q2 = q1 * q2;
	q1q3 = q1 * q3;
	q2q2 = q2 * q2;
	q2q3 = q2 * q3;
	q3q3 = q3 * q3;


	// Reference direction of Earth's magnetic field
	hx = mx*q0q0 - _2q0my*q3 + _2q0mz*q2 + mx*q1q1 + _2q1*my*q2 + _2q1*mz*q3 - mx*q2q2- mx*q3q3;
	hy = _2q0mx*q3 + my*q0q0 - _2q0mz*q1 + _2q1mx*q2 - my*q1q1 + my*q2q2 + _2q2*mz*q3 - my*q3q3;

	_2bx = squareroot(hx * hx + hy * hy);
	_2bz = -_2q0mx*q2 + _2q0my*q1 + mz*q0q0 + _2q1mx*q3 - mz*q1q1 + _2q2*my*q3 - mz*q2q2 + mz*q3q3;

	_4bx	= 2.0f * _2bx;
	_4bz	= 2.0f * _2bz;


	// Gradient decent algorithm corrective step
	s0	= -_2q2 * (2.0f * q1q3 - _2q0q2 - ax)
		+ _2q1 * (2.0f * q0q1 + _2q2q3 - ay)
		- _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
		+ (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
		+ _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);

	s1	= _2q3 * (2.0f * q1q3 - _2q0q2 - ax)
		+ _2q0 * (2.0f * q0q1 + _2q2q3 - ay)
		- 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az)
		+ _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
		+ (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
		+ (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);

	s2	= -_2q0 * (2.0f * q1q3 - _2q0q2 - ax)
		+ _2q3 * (2.0f * q0q1 + _2q2q3 - ay)
		- 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az)
		+ (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
		+ (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
		+ (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);

	s3	= _2q1 * (2.0f * q1q3 - _2q0q2 - ax)
		+ _2q2 * (2.0f * q0q1 + _2q2q3 - ay)
		+ (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
		+ (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
		+ _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);


	// normalise step magnitude	// Apply feedback step
	norm = squareroot(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
	qDot1 -= beta * s0 / norm;
	qDot2 -= beta * s1 / norm;
	qDot3 -= beta * s2 / norm;
	qDot4 -= beta * s3 / norm;


	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * T;
	q1 += qDot2 * T;
	q2 += qDot3 * T;
	q3 += qDot4 * T;


	// Normalise quaternion
	norm = squareroot(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 /= norm;
	q1 /= norm;
	q2 /= norm;
	q3 /= norm;


	//eulerian[0] = arctan2(2*(q0*q1+q2*q3),1-2*(q1*q1+q2*q2))*180/3.141592653;
	//eulerian[1] = arctan2(2*(q0*q3+q1*q2),1-2*(q2*q2+q3*q3))*180/3.141592653;
	//eulerian[2] = arcsin(2*q0*q2 - 2*q1*q3)*180/3.141592653;
	//say("ahrs:	%lf	%lf	%lf\n", eulerian[0], eulerian[1], eulerian[2]);

}




int madgwick_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct element* ele;
	struct relation* rel;
	say("@madgwick_read\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	rel = ele->irel0;
	while(1){
		if(0 == rel)break;
		if(_art_ == rel->srctype){
			arteryread((void*)&rel->srcchip, (void*)&rel->dstchip, buf, len);
		}
		rel = samedstnextsrc(rel);
	}

	return 0;
}
int madgwick_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct element* ele;
	struct relation* rel;
	say("@madgwick_write\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	rel = ele->orel0;
	while(1){
		if(0 == rel)break;
		if(_win_ == rel->dsttype){
			arenawrite((void*)&rel->dstchip, (void*)&rel->srcchip, buf, len);
		}
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int madgwick_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int madgwick_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int madgwick_delete(struct element* ele)
{
	return 0;
}
int madgwick_create(struct element* ele, u8* url)
{
	say("@madgwick_create\n");

	q0 = 1.0;
	q1 = q2 = q3 = 0.0;
	integralx = integraly = integralz = 0.0;
	return 1;
}
