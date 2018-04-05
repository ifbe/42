#include "actor.h"
void quaternionoperation(float*, float*, float);




void camera_deltax(struct arena* win, float delta)
{
	float c,s;
	float x,y,z;
	float cx,cy,cz;		//camera
	float tx,ty,tz;		//target
	float vx,vy,vz;		//vector(tar to cam)

	//camera
	cx = win->cx;
	cy = win->cy;
	cz = win->cz;

	//vector = -front
	vx = -(win->fx);
	vy = -(win->fy);
	vz = -(win->fz);

	//target = camera-front
	tx = cx-vx;
	ty = cy-vy;
	tz = cz-vz;

	c = cosine(delta);
	s = sine(delta);

	//rotate
	x = vx*c + vy*s;
	y = -vx*s + vy*c;
	z = vz;

	//camera = target+vector
	win->cx = tx+x;
	win->cy = ty+y;
	win->cz = tz+z;

	//front = -vector
	win->fx = -x;
	win->fy = -y;
	win->fz = -z;
}
void camera_deltay(struct arena* win, float delta)
{
	float v[4];
	float q[4];
	float cx,cy,cz;		//camera
	float tx,ty,tz;		//target

	//camera
	cx = win->cx;
	cy = win->cy;
	cz = win->cz;

	//vector = -front
	v[0] = -(win->fx);
	v[1] = -(win->fy);
	v[2] = -(win->fz);

	//target = camera-front
	tx = cx-v[0];
	ty = cy-v[1];
	tz = cz-v[2];

	//right = cross(front, (0,0,1))
	q[0] = v[1]*1 - v[2]*0;
	q[1] = v[2]*0 - v[0]*1;
	q[2] = v[0]*0 - v[1]*0;
	quaternionoperation(v, q, delta);

	//camera = target+vector
	win->cx = tx+v[0];
	win->cy = ty+v[1];
	win->cz = tz+v[2];

	//front = -vector
	win->fx = -v[0];
	win->fy = -v[1];
	win->fz = -v[2];
}
void camera_deltaxy(struct arena* win, int dx, int dy)
{
	float delta;

	if(dx > 0)delta = 0.02;
	else delta = -0.02;
	camera_deltax(win, delta);

	if(dy > 0)delta = 0.02;
	else delta = -0.02;
	camera_deltay(win, delta);
}