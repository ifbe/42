#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void quaternion_operation(float*, float*, float);
void matrixmultiply_4(float*, float*);




void mat4_print(mat4 m)
{
	int x,y;
	for(y=0;y<4;y++)
	{
		say("%f,%f,%f,%f\n",m[y][0], m[y][1], m[y][2], m[y][3]);
	}
}
void fixview(mat4 viewmatrix, struct style* sty)
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float norm;
	float cx = sty->vc[0];
	float cy = sty->vc[1];
	float cz = sty->vc[2];

	//uvn.n = front
	float nx = sty->vn[0];
	float ny = sty->vn[1];
	float nz = sty->vn[2];
	norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//uvn.u = right = cross(front,(0,0,1))
	float ux = sty->vr[0];
	float uy = sty->vr[1];
	float uz = sty->vr[2];
	norm = squareroot(ux*ux + uy*uy + uz*uz);
	ux /= norm;
	uy /= norm;
	uz /= norm;

	//uvn.v = above = cross(right, front)
	float vx = sty->vu[0];
	float vy = sty->vu[1];
	float vz = sty->vu[2];
	norm = squareroot(vx*vx + vy*vy + vz*vz);
	vx /= norm;
	vy /= norm;
	vz /= norm;

	viewmatrix[0][0] = ux;
	viewmatrix[0][1] = uy;
	viewmatrix[0][2] = uz;
	viewmatrix[0][3] = -cx*ux - cy*uy - cz*uz;

	viewmatrix[1][0] = vx;
	viewmatrix[1][1] = vy;
	viewmatrix[1][2] = vz;
	viewmatrix[1][3] = -cx*vx - cy*vy - cz*vz;

	viewmatrix[2][0] = -nx;
	viewmatrix[2][1] = -ny;
	viewmatrix[2][2] = -nz;
	viewmatrix[2][3] = cx*nx + cy*ny + cz*nz;

	viewmatrix[3][0] = 0.0;
	viewmatrix[3][1] = 0.0;
	viewmatrix[3][2] = 0.0;
	viewmatrix[3][3] = 1.0f;
	//mat4_print(viewmatrix);
}
void fixproj(mat4 proj, struct style* sty)
{
	float l = vec3_len(sty->vl);
	float r = vec3_len(sty->vr);
	float b = vec3_len(sty->vb);
	float t = vec3_len(sty->vu);
	float n = vec3_len(sty->vn);
	float f = 1e20;

	vec3 lr, bt;
	lr[0] = sty->vr[0] - sty->vl[0];
	lr[1] = sty->vr[1] - sty->vl[1];
	lr[2] = sty->vr[2] - sty->vl[2];
	bt[0] = sty->vu[0] - sty->vb[0];
	bt[1] = sty->vu[1] - sty->vb[1];
	bt[2] = sty->vu[2] - sty->vb[2];
	if(sty->vl[0]*lr[0] + sty->vl[1]*lr[1] + sty->vl[2]*lr[2] < 0)l = -l;
	if(sty->vr[0]*lr[0] + sty->vr[1]*lr[1] + sty->vr[2]*lr[2] < 0)r = -r;
	if(sty->vb[0]*bt[0] + sty->vb[1]*bt[1] + sty->vb[2]*bt[2] < 0)b = -b;
	if(sty->vu[0]*bt[0] + sty->vu[1]*bt[1] + sty->vu[2]*bt[2] < 0)t = -t;

	proj[0][0] = 2 * n / (r-l);
	proj[0][1] = 0.0;
	proj[0][2] = (r+l) / (r-l);
	proj[0][3] = 0.0;

	proj[1][0] = 0.0;
	proj[1][1] = 2 * n / (t-b);
	proj[1][2] = (t+b) / (t-b);
	proj[1][3] = 0.0;

	proj[2][0] = 0.0;
	proj[2][1] = 0.0;
	proj[2][2] = (n+f) / (n-f);
	proj[2][3] = 2 * f * n / (n-f);

	proj[3][0] = 0.0;
	proj[3][1] = 0.0;
	proj[3][2] = -1.0;
	proj[3][3] = 0.0;
}
void fixmatrix(mat4 m, struct arena* win)
{
	int x;
	mat4 t;

	fixproj(m, &win->camera);
	fixview(t, &win->camera);
	mat4_multiply(m, t);
}




void invproj(mat4 proj, struct style* sty)
{
	float l = vec3_len(sty->vl);
	float r = vec3_len(sty->vr);
	float b = vec3_len(sty->vb);
	float t = vec3_len(sty->vu);
	float n = vec3_len(sty->vn);
	float f = 1e20;

	vec3 lr, bt;
	lr[0] = sty->vr[0] - sty->vl[0];
	lr[1] = sty->vr[1] - sty->vl[1];
	lr[2] = sty->vr[2] - sty->vl[2];
	bt[0] = sty->vu[0] - sty->vb[0];
	bt[1] = sty->vu[1] - sty->vb[1];
	bt[2] = sty->vu[2] - sty->vb[2];
	if(sty->vl[0]*lr[0] + sty->vl[1]*lr[1] + sty->vl[2]*lr[2] < 0)l = -l;
	if(sty->vr[0]*lr[0] + sty->vr[1]*lr[1] + sty->vr[2]*lr[2] < 0)r = -r;
	if(sty->vb[0]*bt[0] + sty->vb[1]*bt[1] + sty->vb[2]*bt[2] < 0)b = -b;
	if(sty->vu[0]*bt[0] + sty->vu[1]*bt[1] + sty->vu[2]*bt[2] < 0)t = -t;

	proj[0][0] = (r-l) / 2.0 / n;
	proj[0][1] = 0.0;
	proj[0][2] = 0.0;
	proj[0][3] = (r+l) / 2.0 / n;

	proj[1][0] = 0.0;
	proj[1][1] = (t-b) / 2.0 / n;
	proj[1][2] = 0.0;
	proj[1][3] = (t+b) / 2.0 / n;

	proj[2][0] = 0.0;
	proj[2][1] = 0.0;
	proj[2][2] = 0.0;
	proj[2][3] = -1.0;

	proj[3][0] = 0.0;
	proj[3][1] = 0.0;
	proj[3][2] = (n-f) / 2.0 / n / f;
	proj[3][3] = (n+f) / 2.0 / n / f;
}
void invview(mat4 viewmatrix, struct style* sty)
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float norm;
	float cx = sty->vc[0];
	float cy = sty->vc[1];
	float cz = sty->vc[2];

	//uvn.n = front
	float nx = sty->vn[0];
	float ny = sty->vn[1];
	float nz = sty->vn[2];
	norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//uvn.u = right = cross(front,(0,0,1))
	float ux = sty->vr[0];
	float uy = sty->vr[1];
	float uz = sty->vr[2];
	norm = squareroot(ux*ux + uy*uy + uz*uz);
	ux /= norm;
	uy /= norm;
	uz /= norm;

	//uvn.v = above cross(right, front)
	float vx = sty->vu[0];
	float vy = sty->vu[1];
	float vz = sty->vu[2];
	norm = squareroot(vx*vx + vy*vy + vz*vz);
	vx /= norm;
	vy /= norm;
	vz /= norm;

	viewmatrix[0][0] = ux;
	viewmatrix[0][1] = vx;
	viewmatrix[0][2] = -nx;
	viewmatrix[0][3] = cx;

	viewmatrix[1][0] = uy;
	viewmatrix[1][1] = vy;
	viewmatrix[1][2] = -ny;
	viewmatrix[1][3] = cy;

	viewmatrix[2][0] = uz;
	viewmatrix[2][1] = vz;
	viewmatrix[2][2] = -nz;
	viewmatrix[2][3] = cz;

	viewmatrix[3][0] = 0.0f;
	viewmatrix[3][1] = 0.0f;
	viewmatrix[3][2] = 0.0f;
	viewmatrix[3][3] = 1.0f;
}
void invmvp(vec3 v, struct arena* win)
{
	float f;
	mat4 p;
	mat4 q;
	vec4 t = {v[0], v[1], v[2], 1.0};

	invview(p, &win->camera);
	invproj(q, &win->camera);
	mat4_multiply(p, q);

	f = p[3][0]*t[0] + p[3][1]*t[1] + p[3][2]*t[2] + p[3][3]*t[3];
	v[0] = (p[0][0]*t[0] + p[0][1]*t[1] + p[0][2]*t[2] + p[0][3]*t[3]) / f;
	v[1] = (p[1][0]*t[0] + p[1][1]*t[1] + p[1][2]*t[2] + p[1][3]*t[3]) / f;
	v[2] = (p[2][0]*t[0] + p[2][1]*t[1] + p[2][2]*t[2] + p[2][3]*t[3]) / f;
}