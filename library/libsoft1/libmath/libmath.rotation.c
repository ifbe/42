#define PI 3.14159265358979323846264338327950
double squareroot(double);
double getsin(double);
double arcsin(double);
double getcos(double);
double arccos(double);
double gettan(double);
double arctanyx(double, double);




//q <- q
void quaternion_normalize(float* q)
{
	float invsqrt = 1.0 / squareroot(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
	q[0] *= invsqrt;
	q[1] *= invsqrt;
	q[2] *= invsqrt;
	q[3] *= invsqrt;
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
	d[0] =-s[0] / n2;
	d[1] =-s[1] / n2;
	d[2] =-s[2] / n2;
	d[3] = s[3] / n2;
}
//l <- l, r
void quaternion_multiplyfrom(float* o, float* l, float* r)
{
	o[0] = l[0]*r[3] + l[3]*r[0] + l[1]*r[2] - l[2]*r[1];
	o[1] = l[1]*r[3] + l[3]*r[1] + l[2]*r[0] - l[0]*r[2];
	o[2] = l[2]*r[3] + l[3]*r[2] + l[0]*r[1] - l[1]*r[0];
	o[3] = l[3]*r[3] - l[0]*r[0] - l[1]*r[1] - l[2]*r[2];
}
void quaternion_multiply(float* l, float* r)
{
	float t[4] = {l[0],l[1],l[2],l[3]};
	quaternion_multiplyfrom(l, t, r);
}
void quaternion_rotatefrom(float* o, float* v, float* q)
{
	//t = 2 * cross(q.xyz, v)
	//v' = v + q.w * t + cross(q.xyz, t)
	float t[3];
	t[0] = (q[1]*v[2]-q[2]*v[1]) * 2;
	t[1] = (q[2]*v[0]-q[0]*v[2]) * 2;
	t[2] = (q[0]*v[1]-q[1]*v[0]) * 2;

	o[0] = v[0] + q[3]*t[0] + q[1]*t[2]-q[2]*t[1];
	o[1] = v[1] + q[3]*t[1] + q[2]*t[0]-q[0]*t[2];
	o[2] = v[2] + q[3]*t[2] + q[0]*t[1]-q[1]*t[0];
}
void quaternion_integral(float* q, float* w)
{
#define qx q[0]
#define qy q[1]
#define qz q[2]
#define qw q[3]
#define wx w[0]
#define wy w[1]
#define wz w[2]
	float px = qx;
	float py = qy;
	float pz = qz;
	float pw = qw;
	qx += (  0 *px +wz *py -wy *pz +wx *pw);
	qy += (-wz *px + 0 *py +wx *pz +wy *pw);
	qz += ( wy *px -wx *py + 0 *px +wz *pw);
	qw += (-wx *px -wy *py -wz *pz + 0 *pw);
}
//v <- v, q
void quaternion_rotate(float* v, float* q)
{
	quaternion_rotatefrom(v, v, q);
}
void quaternion_slerp(float* out,float* q0,float* q1,float t)
{
}




//in(qx,qy,qz,qw) -> out(vx,vy,vz)
void quaternion2axismulangle(float* q, float* a)
{/*
	float l,t;

	l = q[0]*q[0] + q[1]*q[1] + q[2]*q[2];
	if(l < 1e-12){
		a[0] = 0.0;
		a[1] = 0.0;
		a[2] = 0.0;
	}
	else{
		l = squareroot(l);
		t = 2.0 * arctanyx(l, q[3]) / l;
		a[0] = q[0] * t;
		a[1] = q[1] * t;
		a[2] = q[2] * t;
	}
*/
	float l2 = 1 - q[3]*q[3];	//=q[0]*q[0] + q[1]*q[1] + q[2]*q[2]
	if(l2 < 1e-12){
		a[0] = a[1] = a[2] = 0.0;
	}
	else{
		float angle = 2.0 * arccos(q[3]);
		float angle_invl = angle / squareroot(l2);
		a[0] = q[0] * angle_invl;
		a[1] = q[1] * angle_invl;
		a[2] = q[2] * angle_invl;
	}
}
//out(qx,qy,qz,qw) <- in(vx,vy,vz)
void quaternion4axismulangle(float* q, float* a)
{
	float n2 = a[0]*a[0] + a[1]*a[1] * a[2]*a[2];
	if(n2 < 1e-12){
		q[0] = 0.0;
		q[1] = 0.0;
		q[2] = 0.0;
		q[3] = 1.0;
	}
	else{
		float angle = squareroot(n2);
		float sinbynorm = getsin(angle/2) / angle;
		q[0] = a[0] * sinbynorm;
		q[1] = a[1] * sinbynorm;
		q[2] = a[2] * sinbynorm;
		q[3] = getcos(angle/2);
	}
}
//in(qx,qy,qz,qw) -> out((vx,vy,vz),angle)
void quaternion2axisandangle(float* q, float* axis, float* angle)
{
	float l2 = 1 - q[3]*q[3];	//=q[0]*q[0] + q[1]*q[1] + q[2]*q[2]
	if(l2 < 1e-12){
		axis[0] = axis[1] = 0.0;
		axis[2] = 1.0;
		angle[0] = 0.0;
	}
	else{
		float invl = 1.0 / squareroot(l2);
		axis[0] = q[0] * invl;
		axis[1] = q[1] * invl;
		axis[2] = q[2] * invl;
		*angle = arccos(q[3]) * 2.0;
	}
}
//out(qx,qy,qz,qw) <- in((vx,vy,vz),angle)
void quaternion4axisandangle(float* q, float* a, float angle)
{
	float norm = squareroot(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
	float sinbynorm = getsin(angle/2) / norm;

	q[0] = a[0] * sinbynorm;
	q[1] = a[1] * sinbynorm;
	q[2] = a[2] * sinbynorm;
	q[3] = getcos(angle/2);
}
//in(qx,qy,qz,qw) -> out(pitch_x,roll_y,yaw_z)
void quaternion2eulerian(float* q, float* e)
{
	//atan2(2(xw+yz), 1-2(xx+yy))
	e[0] = arctanyx( (q[0]*q[3]+q[1]*q[2])*2 , 1-(q[0]*q[0]+q[1]*q[1])*2 );
	e[0] *= 180.0/PI;

	//atan2(2(zw+xy), 1-2(yy+zz))
	e[2] = arctanyx( (q[2]*q[3]+q[0]*q[1])*2 , 1-(q[1]*q[1]+q[2]*q[2])*2 );
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
	float cospitch = getcos(pitch);
	float sinpitch = getsin(pitch);
	float cosyaw = getcos(yaw);
	float sinyaw = getsin(yaw);
	float cosroll = getcos(roll);
	float sinroll = getsin(roll);

	q[0] = sinpitch*cosyaw*cosroll - cospitch*sinyaw*sinroll;
	q[1] = cospitch*sinyaw*cosroll + sinpitch*cosyaw*sinroll;
	q[2] = cospitch*cosyaw*sinroll - sinpitch*sinyaw*cosroll;
	q[3] = cospitch*cosyaw*cosroll + sinpitch*sinyaw*sinroll;
}

void quaternion2worldspacebodyaxis(float* q, float* r, float* f, float* t)
{
	if(r){
	r[0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
	r[1] =       (q[0]*q[1] + q[2]*q[3]) * 2.0;
	r[2] =       (q[0]*q[2] - q[1]*q[3]) * 2.0;
	}
	if(f){
	f[0] =       (q[0]*q[1] - q[2]*q[3]) * 2.0;
	f[1] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
	f[2] =       (q[1]*q[2] + q[0]*q[3]) * 2.0;
	}
	if(t){
	t[0] =       (q[0]*q[2] + q[1]*q[3]) * 2.0;
	t[1] =       (q[1]*q[2] - q[0]*q[3]) * 2.0;
	t[2] = 1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0;
	}
}

void quaternion2bodyspaceworldaxis(float* q, float* r, float* f, float* t)
{
	if(r){
	r[0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
	r[1] =       (q[0]*q[1] - q[2]*q[3]) * 2.0;
	r[2] =       (q[0]*q[2] + q[1]*q[3]) * 2.0;
	}
	if(f){
	f[0] =       (q[0]*q[1] + q[2]*q[3]) * 2.0;
	f[1] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
	f[2] =       (q[1]*q[2] - q[0]*q[3]) * 2.0;
	}
	if(t){
	t[0] =       (q[0]*q[2] - q[1]*q[3]) * 2.0;
	t[1] =       (q[1]*q[2] + q[0]*q[3]) * 2.0;
	t[2] = 1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0;
	}
}

//in(qx,qy,qz,qw) -> out(matrix)
void quaternion2matthree(float* q, float* m)
{
	//not change axis: r=m[*][0], f=m[*][1], t=m[*][2]
	//    change axis: r=m[0][*], f=m[1][*], t=m[2][*]
	m[0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
	m[1] =       (q[0]*q[1] - q[2]*q[3]) * 2.0;
	m[2] =       (q[0]*q[2] + q[1]*q[3]) * 2.0;

	m[3] =       (q[0]*q[1] + q[2]*q[3]) * 2.0;
	m[4] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
	m[5] =       (q[1]*q[2] - q[0]*q[3]) * 2.0;

	m[6] =       (q[0]*q[2] - q[1]*q[3]) * 2.0;
	m[7] =       (q[1]*q[2] + q[0]*q[3]) * 2.0;
	m[8] = 1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0;
}
//out(qx,qy,qz,qw) <- in(matrix)
void quaternion4matthree(float* q, float (*m)[3])
{
	float trace = m[0][0] + m[1][1] + m[2][2];
	q[3] = 0.5*squareroot(trace+1.0);
	q[3] = (q[3] > 0.0) ? q[3] : -q[3];
	if(q[3] > 0.000000001){
		q[0] = m[2][1] - m[1][2];
		q[1] = m[0][2] - m[2][0];
		q[2] = m[1][0] - m[0][1];
	}
}
//in(qx,qy,qz,qw) -> out(matrix)
void quaternion2matfour(float* q, float* m)
{
	//not change axis: r=m[*][0], f=m[*][1], t=m[*][2]
	//    change axis: r=m[0][*], f=m[1][*], t=m[2][*]
	m[ 0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
	m[ 1] =       (q[0]*q[1] - q[2]*q[3]) * 2.0;
	m[ 2] =       (q[0]*q[2] + q[1]*q[3]) * 2.0;
	m[ 3] = 0.0;

	m[ 4] =       (q[0]*q[1] + q[2]*q[3]) * 2.0;
	m[ 5] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
	m[ 6] =       (q[1]*q[2] - q[0]*q[3]) * 2.0;
	m[ 7] = 0.0;

	m[ 8] =       (q[0]*q[2] - q[1]*q[3]) * 2.0;
	m[ 9] =       (q[1]*q[2] + q[0]*q[3]) * 2.0;
	m[10] = 1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0;
	m[11] = 0.0;

	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;
}
//out(qx,qy,qz,qw) <- in(matrix)
void quaternion4matfour(float* q, float (*m)[4])
{
	float trace = m[0][0] + m[1][1] + m[2][2];
	q[3] = 0.5*squareroot(trace+1.0);
	q[3] = (q[3] > 0.0) ? q[3] : -q[3];
	if(q[3] > 0.000000001){
		q[0] = m[2][1] - m[1][2];
		q[1] = m[0][2] - m[2][0];
		q[2] = m[1][0] - m[0][1];
	}
}
void quaternion_aa(float* v, float* aa)
{
	float q[4];
	quaternion4axismulangle(q, aa);
	quaternion_rotate(v, q);
}
void quaternion_operation(float* v, float* axis, float angle)
{
	float q[4];
	quaternion4axisandangle(q, axis, angle);
	quaternion_rotate(v, q);
}




//in(vx,vy,vz) -> out(matrix)
void axismulangle2matthree(float* a, float (*m)[3])
{
	//Rodrigues
}
//out(vx,vy,vz) <- in(matrix)
void axismulangle4matthree(float* a, float (*m)[3])
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
void axismulangle2quaternion(float* a, float* q)
{
	quaternion4axismulangle(q, a);
}
//out(vx,vy,vz) <- in(qx,qy,qz,qw)
void axismulangle4quaternion(float* a, float* q)
{
	quaternion2axismulangle(q, a);
}
//in(vx,vy,vz) -> out((vx,vy,vz),angle)
void axismulangle2axisandangle(float* a, float* axis, float* angle)
{
	float tmp = squareroot(a[0]*a[0] + a[1]*a[1] * a[2]*a[2]);
	*angle = tmp;

	tmp = 1.0 / tmp;
	axis[0] = a[0] * tmp;
	axis[1] = a[1] * tmp;
	axis[2] = a[2] * tmp;
}
//out(vx,vy,vz) <- in((vx,vy,vz),angle)
void axismulangle4axisandangle(float* a, float* axis, float angle)
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




//in(m3) -> out(vx,vy,vz)
void matthree2axismulangle(float (*m)[3], float* a)
{
	axismulangle4matthree(a, m);
}
//out(m3) -> in(vx,vy,vz)
void matthree4axismulangle(float (*m)[3], float* a)
{
	axismulangle2matthree(a, m);
}
//in(m3) -> out(qx,qy,qz,qw)
void matthree2quaternion(float (*m)[3], float* q)
{
	quaternion4matthree(q, m);
}
//out(m3) <- in(qx,qy,qz,qw)
void matthree4quaternion(float* m, float* q)
{
	quaternion2matthree(q, m);
}
//in(m3) -> out(m4)
void matthree2matfour(float* m3, float* m4)
{
	m4[ 0] = m3[0];
	m4[ 1] = m3[1];
	m4[ 2] = m3[2];
	m4[ 3] = 0.0;

	m4[ 4] = m3[3];
	m4[ 5] = m3[4];
	m4[ 6] = m3[5];
	m4[ 7] = 0.0;

	m4[ 8] = m3[6];
	m4[ 9] = m3[7];
	m4[10] = m3[8];
	m4[11] = 0.0;

	m4[12] = 0.0;
	m4[13] = 0.0;
	m4[14] = 0.0;
	m4[15] = 1.0;
}
//out(m3) <- in(m4)
void matthree4matfour(float* m3, float* m4)
{
	m3[0] = m4[0];
	m3[1] = m4[1];
	m3[2] = m4[2];

	m3[3] = m4[4];
	m3[4] = m4[5];
	m3[5] = m4[6];

	m3[6] = m4[8];
	m3[7] = m4[9];
	m3[8] = m4[10];
}




//in(m4) -> out(qx,qy,qz,qw)
void matfour2quaternion(float (*m)[4], float* q)
{
	quaternion4matfour(q, m);
}
//out(m4) <- in(qx,qy,qz,qw)
void matfour4quaternion(float* m, float* q)
{
	quaternion2matfour(q, m);
}
//in(m4) -> out(m3)
void matfour2matthree(float* m4, float* m3)
{
	matthree4matfour(m3, m4);
}
//out(m4) <- in(m3)
void matfour4matthree(float* m4, float* m3)
{
	matthree2matfour(m3, m4);
}
