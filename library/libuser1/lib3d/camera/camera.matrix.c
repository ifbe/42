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
void fixview(mat4 viewmatrix, struct arena* win)
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float norm;
	float cx = win->camera.vc[0];
	float cy = win->camera.vc[1];
	float cz = win->camera.vc[2];

	//uvn.n = front
	float nx = win->camera.vf[0];
	float ny = win->camera.vf[1];
	float nz = win->camera.vf[2];
	norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//uvn.u = right = cross(front,(0,0,1))
	float ux = ny*1 - nz*0;
	float uy = nz*0 - nx*1;
	float uz = nx*0 - ny*0;
	norm = squareroot(ux*ux + uy*uy + uz*uz);
	ux /= norm;
	uy /= norm;
	uz /= norm;

	//uvn.v = above = cross(right, front)
	float vx = uy*nz - uz*ny;
	float vy = uz*nx - ux*nz;
	float vz = ux*ny - uy*nx;
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
void fixproj(mat4 projmatrix, struct arena* win)
{
	float n = win->neardepth;
	float t = win->nearstride;
	float w = (float)(win->width);
	float h = (float)(win->height);

	t = 1.0 / t;

	projmatrix[0][0] = t * h / w;
	projmatrix[0][1] = 0.0;
	projmatrix[0][2] = 0.0;
	projmatrix[0][3] = 0.0;

	projmatrix[1][0] = 0.0;
	projmatrix[1][1] = t;
	projmatrix[1][2] = 0.0;
	projmatrix[1][3] = 0.0;

	projmatrix[2][0] = 0.0;
	projmatrix[2][1] = 0.0;
	projmatrix[2][2] = -1.0;	//	(n+f) / (n-f);
	projmatrix[2][3] = -2*n;	//	2*n*f / (n-f);

	projmatrix[3][0] = 0.0;
	projmatrix[3][1] = 0.0;
	projmatrix[3][2] = -1.0;
	projmatrix[3][3] = 0.0;
}
void fixmatrix(mat4 mvp, struct arena* win)
{
	int x;
	mat4 m;

	fixproj(mvp, win);
	fixview(m, win);
	mat4_multiply(mvp, m);
}




void invproj(mat4 projmatrix, struct arena* win)
{
	float n = win->neardepth;
	float t = win->nearstride;
	float w = (float)(win->width);
	float h = (float)(win->height);

	projmatrix[0][0] = t * w / h;
	projmatrix[0][1] = 0.0;
	projmatrix[0][2] = 0.0;
	projmatrix[0][3] = 0.0;

	projmatrix[1][0] = 0.0;
	projmatrix[1][1] = t;
	projmatrix[1][2] = 0.0;
	projmatrix[1][3] = 0.0;

	projmatrix[2][0] = 0.0;
	projmatrix[2][1] = 0.0;
	projmatrix[2][2] = 0.0;
	projmatrix[2][3] = -1.0;

	projmatrix[3][0] = 0.0;
	projmatrix[3][1] = 0.0;
	projmatrix[3][2] = -0.5/n;
	projmatrix[3][3] = 0.5/n;
}
void invview(mat4 viewmatrix, struct arena* win)
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float norm;
	float cx = win->camera.vc[0];
	float cy = win->camera.vc[1];
	float cz = win->camera.vc[2];

	//uvn.n = front
	float nx = win->camera.vf[0];
	float ny = win->camera.vf[1];
	float nz = win->camera.vf[2];
	norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//uvn.u = right = cross(front,(0,0,1))
	float ux = ny*1 - nz*0;
	float uy = nz*0 - nx*1;
	float uz = nx*0 - ny*0;
	norm = squareroot(ux*ux + uy*uy + uz*uz);
	ux /= norm;
	uy /= norm;
	uz /= norm;

	//uvn.v = above cross(right, front)
	float vx = uy*nz - uz*ny;
	float vy = uz*nx - ux*nz;
	float vz = ux*ny - uy*nx;
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

	invview(p, win);
	invproj(q, win);
	mat4_multiply(p, q);

	f = p[3][0]*t[0] + p[3][1]*t[1] + p[3][2]*t[2] + p[3][3]*t[3];
	v[0] = (p[0][0]*t[0] + p[0][1]*t[1] + p[0][2]*t[2] + p[0][3]*t[3]) / f;
	v[1] = (p[1][0]*t[0] + p[1][1]*t[1] + p[1][2]*t[2] + p[1][3]*t[3]) / f;
	v[2] = (p[2][0]*t[0] + p[2][1]*t[1] + p[2][2]*t[2] + p[2][3]*t[3]) / f;
}