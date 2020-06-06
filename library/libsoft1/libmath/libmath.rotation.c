#define PI 3.14159265358979323846264338327950
double squareroot(double);
double sine(double);
double arcsin(double);
double cosine(double);
double arccos(double);
double tangent(double);
double arctan2(double, double);




//q <- q
void quaternion_normalize(float* q)
{
	float norm = squareroot(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
	q[0] /= norm;
	q[1] /= norm;
	q[2] /= norm;
	q[3] /= norm;
}
//s -> d
void quaternion_conjugate(float* s, float* d)
{
	d[0] = -s[0];
	d[1] = -s[1];
	d[2] = -s[2];
	d[3] = s[3];
}
//s -> d
void quaternion_inverse(float* s, float* d)
{
	float n2 = s[0]*s[0] + s[1]*s[1] + s[2]*s[2] + s[3]*s[3];
	d[0] = -s[0] / n2;
	d[1] = -s[1] / n2;
	d[2] = -s[2] / n2;
	d[3] = s[3] / n2;
}
//l <- l, r
void quaternion_multiply(float* l, float* r)
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
//v <- v, q
void quaternion_rotate(float* v, float* q)
{
	//t = 2 * cross(q.xyz, v)
	//v' = v + q.w * t + cross(q.xyz, t)
	float t[3];
	t[0] = (q[1]*v[2]-q[2]*v[1]) * 2;
	t[1] = (q[2]*v[0]-q[0]*v[2]) * 2;
	t[2] = (q[0]*v[1]-q[1]*v[0]) * 2;

	v[0] += q[3]*t[0] + q[1]*t[2]-q[2]*t[1];
	v[1] += q[3]*t[1] + q[2]*t[0]-q[0]*t[2];
	v[2] += q[3]*t[2] + q[0]*t[1]-q[1]*t[0];
}




//in(qx,qy,qz,qw) -> out(vx,vy,vz)
void quaternion2axisangle(float* q, float* a)
{
	float t = arccos(q[3]) * 2.0;
	t /= squareroot(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);

	a[0] = q[0] * t;
	a[1] = q[1] * t;
	a[2] = q[2] * t;
}
//out(qx,qy,qz,qw) <- in(vx,vy,vz)
void quaternion4axisangle(float* q, float* a)
{
	float angle = squareroot(a[0]*a[0] + a[1]*a[1] * a[2]*a[2]);
	float sinbynorm = sine(angle/2) / angle;
	q[0] = a[0] * sinbynorm;
	q[1] = a[1] * sinbynorm;
	q[2] = a[2] * sinbynorm;
	q[3] = cosine(angle/2);
}
//in(qx,qy,qz,qw) -> out((vx,vy,vz),angle)
void quaternion2axisandangle(float* q, float* axis, float* angle)
{
	float n = 1.0 / squareroot(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
	axis[0] = q[0] * n;
	axis[1] = q[1] * n;
	axis[2] = q[2] * n;
	*angle = arccos(q[3]) * 2.0;
}
//out(qx,qy,qz,qw) <- in((vx,vy,vz),angle)
void quaternion4axisandangle(float* q, float* a, float angle)
{
	float norm = squareroot(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
	float sinbynorm = sine(angle/2) / norm;

	q[0] = a[0] * sinbynorm;
	q[1] = a[1] * sinbynorm;
	q[2] = a[2] * sinbynorm;
	q[3] = cosine(angle/2);
}
//in(qx,qy,qz,qw) -> out(pitch_x,roll_y,yaw_z)
void quaternion2eulerian(float* q, float* e)
{
	//atan2(2(xw+yz), 1-2(xx+yy))
	e[0] = arctan2( (q[0]*q[3]+q[1]*q[2])*2 , 1-(q[0]*q[0]+q[1]*q[1])*2 );
	e[0] *= 180.0/PI;

	//atan2(2(zw+xy), 1-2(yy+zz))
	e[2] = arctan2( (q[2]*q[3]+q[0]*q[1])*2 , 1-(q[1]*q[1]+q[2]*q[2])*2 );
	e[2] *= 180.0/PI;

	//asin(2(yw-xz))
	e[1] = arcsin(  (q[1]*q[3]-q[0]*q[2])*2 );
	e[1] *= 180.0/PI;
}
//out(qx,qy,qz,qw) <- in(pitch_x,roll_y,yaw_z)
void quaternion4eulerian(float* q, float* e)
{
	float pitch = e[0]*PI/360.0;
	float yaw = e[1]*PI/360.0;
	float roll = e[2]*PI/360.0;
	float cospitch = cosine(pitch);
	float sinpitch = sine(pitch);
	float cosyaw = cosine(yaw);
	float sinyaw = sine(yaw);
	float cosroll = cosine(roll);
	float sinroll = sine(roll);

	q[0] = sinpitch*cosyaw*cosroll - cospitch*sinyaw*sinroll;
	q[1] = cospitch*sinyaw*cosroll + sinpitch*cosyaw*sinroll;
	q[2] = cospitch*cosyaw*sinroll - sinpitch*sinyaw*cosroll;
	q[3] = cospitch*cosyaw*cosroll + sinpitch*sinyaw*sinroll;
}
//in(qx,qy,qz,qw) -> out(matrix)
void quaternion2matrix(float* q, float* m)
{
	//1-2*(y*y+z*z), 2*(x*y-z*w), 2*(x*z+y*w)
	//2*(x*y+z*w), 1-2*(x*x+z*z), 2*(y*z-x*w)
	//2*(x*z-y*w), 2*(y*z+x*w), 1-2*(x*x+y*y)
	m[ 0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
	m[ 1] = 2.0 * (q[0]*q[1] + q[2]*q[3]);
	m[ 2] = 2.0 * (q[0]*q[2] - q[1]*q[3]);
	m[ 3] = 0.0;

	m[ 4] = 2.0 * (q[0]*q[1] - q[2]*q[3]);
	m[ 5] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
	m[ 6] = 2.0 * (q[1]*q[2] + q[0]*q[3]);
	m[ 7] = 0.0;

	m[ 8] = 2.0 * (q[0]*q[2] + q[1]*q[3]);
	m[ 9] = 2.0 * (q[1]*q[2] - q[0]*q[3]);
	m[10] = 1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0;
	m[11] = 0.0;

	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;
}
//out(qx,qy,qz,qw) <- in(matrix)
void quaternion4matrix(float* q, float* m)
{
}




//in(vx,vy,vz) -> out(matrix)
void axisangle2matrix(float* a, float (*m)[3])
{
	//Rodrigues
}
//out(vx,vy,vz) <- in(matrix)
void axisangle4matrix(float* a, float (*m)[3])
{
	float n;
	a[0] = m[2][1] - m[1][2];
	a[1] = m[0][2] - m[2][0];
	a[2] = m[1][0] - m[0][1];

	n = arccos((m[0][0]+m[1][1]+m[2][2]-1.0) / 2.0);
	n/= squareroot(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
	a[0] *= n;
	a[1] *= n;
	a[2] *= n;
}
//in(vx,vy,vz) -> out(qx,qy,qz,qw)
void axisangle2quaternion(float* a, float* q)
{
	quaternion4axisangle(q, a);
}
//out(vx,vy,vz) <- in(qx,qy,qz,qw)
void axisangle4quaternion(float* a, float* q)
{
	quaternion2axisangle(q, a);
}
//in(vx,vy,vz) -> out((vx,vy,vz),angle)
void axisangle2axisandangle(float* a, float* axis, float* angle)
{
	float tmp = squareroot(a[0]*a[0] + a[1]*a[1] * a[2]*a[2]);
	*angle = tmp;

	tmp = 1.0 / tmp;
	axis[0] = a[0] * tmp;
	axis[1] = a[1] * tmp;
	axis[2] = a[2] * tmp;
}
//out(vx,vy,vz) <- in((vx,vy,vz),angle)
void axisangle4axisandangle(float* a, float* axis, float angle)
{
	float tmp = angle * squareroot(axis[0]*axis[0] + axis[1]*axis[1] * axis[2]*axis[2]);
	a[0] = axis[0] * tmp;
	a[1] = axis[1] * tmp;
	a[2] = axis[2] * tmp;
}




//in((vx,vy,vz),angle) -> out(qx,qy,qz,qw)
void axisandangle2quaternion(float* axis, float angle, float* q)
{
	quaternion4axisandangle(q, axis, angle);
}
//out((vx,vy,vz),angle) <- in(qx,qy,qz,qw)
void axisandangle4quaternion(float* axis, float* angle, float* q)
{
	quaternion2axisandangle(q, axis, angle);
}
void quaternion_operation(float* v, float* axis, float angle)
{
	float q[4];
	quaternion4axisandangle(q, axis, angle);
	quaternion_rotate(v, q);
}




//in(pitch_x,roll_y,yaw_z) -> out(qx,qy,qz,qw)
void eulerian2quaternion(float* e, float* q)
{
	quaternion4eulerian(q, e);
}
//out(pitch_x,roll_y,yaw_z) <- in(qx,qy,qz,qw)
void eulerian4quaternion(float* e, float* q)
{
	quaternion2eulerian(q, e);
}




//in(matrix) -> out(qx,qy,qz,qw)
void matrix2quaternion(float* m, float* q)
{
	quaternion4matrix(q, m);
}
//out(matrix) <- in(qx,qy,qz,qw)
void matrix4quaternion(float* m, float* q)
{
	quaternion2matrix(q, m);
}