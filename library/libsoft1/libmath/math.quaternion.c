#include<stdio.h>
#include<math.h>
#define PI 3.14159265358979323846264338327950




//in:	vx,vy,vz,angle
//out:	qx,qy,qz,qw
void axis2quaternion(float* av, float* q)
{
	//normalize vector
	float angle = av[3]*PI/360.0;
	float norm = sqrt(av[0]*av[0] + av[1]*av[1] + av[2]*av[2]);
	float sinbynorm = sin(angle)/norm;

	//create quaternion
	q[0] = av[0]*sinbynorm;
	q[1] = av[1]*sinbynorm;
	q[2] = av[2]*sinbynorm;
	q[3] = cos(angle);
}
//in:	pitch,yaw,roll
//out:	qx,qy,qz,qw
void eulerian2quaternion(float* eulerian, float* q)
{
	float pitch = eulerian[0]*PI/360.0;
	float yaw = eulerian[1]*PI/360.0;
	float roll = eulerian[2]*PI/360.0;
	float cospitch = cos(pitch);
	float sinpitch = sin(pitch);
	float cosyaw = cos(yaw);
	float sinyaw = sin(yaw);
	float cosroll = cos(roll);
	float sinroll = sin(roll);

	q[0] = sinpitch*cosyaw*cosroll - cospitch*sinyaw*sinroll;
	q[1] = cospitch*sinyaw*cosroll + sinpitch*cosyaw*sinroll;
	q[2] = cospitch*cosyaw*sinroll - sinpitch*sinyaw*cosroll;
	q[3] = cospitch*cosyaw*cosroll + sinpitch*sinyaw*sinroll;
}
//in:	matrix
//out:	qx,qy,qz,qw
void matrix2quaternion(float* matrix, float* q)
{
}




//in:	qx,qy,qz,qw
//out:	vx,vy,vz,angle
void quaternion2axis(float* q, float* av)
{
	float angle = acos(q[3]);
	float scale = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);

	av[0] = q[0]/scale;
	av[1] = q[1]/scale;
	av[2] = q[2]/scale;
	av[3] = angle*360/3.141592653;
}
//in:	qx,qy,qz,qw
//out:	pitch,yaw,roll
void quaternion2eulerian(float* q, float* eulerian)
{
	eulerian[0] = atan2( (q[3]*q[1]+q[2]*q[0])*2 , 1-(q[1]*q[1]+q[2]*q[2])*2 );
	eulerian[0] *= 180.0/PI;
	eulerian[1] = atan2( (q[3]*q[0]+q[1]*q[2])*2 , 1-(q[2]*q[2]+q[0]*q[0])*2 );
	eulerian[1] *= 180.0/PI;
	eulerian[2] = asin(  (q[3]*q[2]-q[1]*q[0])*2 );
	eulerian[2] *= 180.0/PI;
}
//in:	qx,qy,qz,qw
//out:	matrix
void quaternion2matrix(float* q, float* matrix)
{
	matrix[0] = (q[3]*q[3] + q[1]*q[1] - q[2]*q[2] - q[0]*q[0]);
	matrix[1] = (q[1]*q[2] - q[3]*q[0])*2;
	matrix[2] = (q[3]*q[2] + q[1]*q[0])*2;

	matrix[3] = (q[3]*q[0] + q[1]*q[2])*2;
	matrix[4] = (q[3]*q[3] - q[1]*q[1] + q[2]*q[2] - q[0]*q[0]);
	matrix[5] = (q[2]*q[0] - q[3]*q[1])*2;

	matrix[6] = (q[1]*q[0] - q[3]*q[2])*2;
	matrix[7] = (q[3]*q[1] + q[2]*q[0])*2;
	matrix[8] = (q[3]*q[3] - q[1]*q[1] - q[2]*q[2] + q[0]*q[3]);
}




void quaternionnormalize(float* p)
{
	float norm = sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2] + p[3]*p[3]);
	p[0] /= norm;
	p[1] /= norm;
	p[2] /= norm;
	p[3] /= norm;
}
//in:	left, right
//out:	left = left*right
void quaternionmultiply(float* l, float* r)
{
	float t[4];
	t[0] = l[0];
	t[1] = l[1];
	t[2] = l[2];
	t[3] = l[3];

	l[0] = t[3]*r[0] + t[0]*r[3] + t[1]*r[2] - t[2]*r[1];
	l[1] = t[3]*r[1] - t[0]*r[2] + t[1]*r[3] + t[2]*r[0];
	l[2] = t[3]*r[2] + t[0]*r[1] - t[1]*r[0] + t[2]*r[3];
	l[3] = t[3]*r[3] - t[0]*r[0] - t[1]*r[1] - t[2]*r[2];
}
//in:	vector, quaternion
//out:	vector
void quaternionrotate(float* v, float* q)
{
	//t = 2 * cross(q.xyz, v)
	//v' = v + q.w * t + cross(q.xyz, t)
	//float tx = 2*q[2]*v[2]
	float j[3];
	float k[3];
	j[0] = (q[1]*v[2]-q[2]*v[1]) * 2;
	j[1] = (q[2]*v[0]-q[0]*v[2]) * 2;
	j[2] = (q[0]*v[1]-q[1]*v[0]) * 2;

	k[0] = q[1]*j[2]-q[2]*j[1];
	k[1] = q[2]*j[0]-q[0]*j[2];
	k[2] = q[0]*j[1]-q[1]*j[0];

	v[0] += q[3]*j[0] + k[0];
	v[1] += q[3]*j[1] + k[1];
	v[2] += q[3]*j[2] + k[2];
}
