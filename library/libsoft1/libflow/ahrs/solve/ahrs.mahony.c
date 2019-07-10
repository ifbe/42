#include "libsoft.h"
double squareroot(double);

#define T 0.001
#define Kp 1.0
#define Ki 0.001

//
float q0,q1,q2,q3;
float integralx, integraly, integralz;




//https://github.com/PaulStoffregen/MahonyAHRS
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
void mahonyupdate6(
	float gx, float gy, float gz,
	float ax, float ay, float az)
{
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// Convert gyroscope degrees/sec to radians/sec
	gx *= 0.0174533f;
	gy *= 0.0174533f;
	gz *= 0.0174533f;

	// Compute feedback only if accelerometer measurement valid
	// (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Estimated direction of gravity
		halfvx = q1 * q3 - q0 * q2;
		halfvy = q0 * q1 + q2 * q3;
		halfvz = q0 * q0 - 0.5f + q3 * q3;

		// Error is sum of cross product between estimated
		// and measured direction of gravity
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled
		if(Ki > 0.0f) {
			// integral error scaled by Ki
			integralx += Ki * halfex * T;
			integraly += Ki * halfey * T;
			integralz += Ki * halfez * T;
			gx += integralx;	// apply integral feedback
			gy += integraly;
			gz += integralz;
		} else {
			integralx = 0.0f;	// prevent integral windup
			integraly = 0.0f;
			integralz = 0.0f;
		}

		// Apply proportional feedback
		gx += Kp * halfex;
		gy += Kp * halfey;
		gz += Kp * halfez;
	}

	// Integrate rate of change of quaternion
	gx *= (0.5f * T);		// pre-multiply common factors
	gy *= (0.5f * T);
	gz *= (0.5f * T);
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx);

	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}
void mahonyupdate9(
	float ax, float ay, float az,
	float gx, float gy, float gz,
	float mx, float my, float mz)
{
	float norm;
	float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;  
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;


	// Normalise acc, mag
	norm = squareroot(ax * ax + ay * ay + az * az);
	ax /= norm;
	ay /= norm;
	az /= norm;     

	norm = squareroot(mx * mx + my * my + mz * mz);
	mx /= norm;
	my /= norm;
	mz /= norm;   


	// Auxiliary variables to avoid repeated arithmetic
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
	hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
	hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));

	bx = squareroot(hx * hx + hy * hy);
	bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));


	// Estimated direction of gravity and magnetic field
	halfvx = q1q3 - q0q2;
	halfvy = q0q1 + q2q3;
	halfvz = q0q0 - 0.5f + q3q3;

	halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
	halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
	halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);  


	// Error is sum of cross product between estimated direction and measured direction of field vectors
	halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
	halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
	halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);


	// Compute and apply integral feedback if enabled
	if(Ki > 0.0f)
	{
		// integral error scaled by Ki
		integralx += Ki * halfex * T;
		integraly += Ki * halfey * T;
		integralz += Ki * halfez * T;

		gx += integralx;	// apply integral feedback
		gy += integraly;
		gz += integralz;
	}
	else
	{
		integralx = 0.0f;	// prevent integral windup
		integraly = 0.0f;
		integralz = 0.0f;
	}

	// Apply proportional feedback
	gx += Kp * halfex;
	gy += Kp * halfey;
	gz += Kp * halfez;
	

	// Integrate rate of change of quaternion
	gx *= 0.5f * T;		// pre-multiply common factors
	gy *= 0.5f * T;
	gz *= 0.5f * T;
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx); 
	

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




int mahony_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct element* ele;
	struct relation* rel;
	say("@mahony_read\n");

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
int mahony_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct element* ele;
	struct relation* rel;
	say("@mahony_write\n");

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
int mahony_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int mahony_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int mahony_delete(struct element* ele)
{
	return 0;
}
int mahony_create(struct element* ele, u8* url)
{
	say("@mahony_create\n");

	q0 = 1.0;
	q1 = q2 = q3 = 0.0;
	integralx = integraly = integralz = 0.0;
	return 1;
}
