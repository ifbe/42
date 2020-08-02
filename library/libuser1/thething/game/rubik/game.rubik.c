#include "libuser.h"
#define level 4
#define CODE data2
#define TIME data3
void rubikscube_generate(void*, int);
void rubikscube_solve(void*, int);
void dx11solid_rect(struct entity* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);




//green, blue, red, orange, yellow, white
u32 rubikcolor[6] = {0x00ff00,0x0000ff,0xff0000,0xfa8010,0xffff00,0xffffff};




//left, right, near, far, bottom, upper
static int rubikscube_import(char* file, u8 (*buf)[4][4])
{
	int x,y;
	int j,t;
	u8 tmp[0x100];
	j = openreadclose(file, 0, tmp, 0x100);
	//printmemory(tmp, 0x100);
	if(j<=0)return 0;

	x = y = 0;
	for(j=0;j<0x100;j++)
	{
		if(0xd == tmp[j])continue;
		if(0xa == tmp[j]){
			x = 0;y += 1;
			if(y >= 6)break;
			continue;
		}
		if(x<16)
		{
			t = tmp[j];
			switch(t)
			{
				case 'g':t = 0x30;break;
				case 'b':t = 0x31;break;
				case 'r':t = 0x32;break;
				case 'o':t = 0x33;break;
				case 'y':t = 0x34;break;
				case 'w':t = 0x35;break;
			}
			if((t >= 0x30)&&(t <= 0x35))
			{
				buf[y][x/4][x%4] = t - 0x30;
				x++;
			}
		}
	}

	//printmemory(buf, 81);
	return 1;
}
static void rubikscube_rotateface(u8* buf)
{
	u8 t;
	int j,v;
	for(v=0;v<level/2;v++){
		for(j=0;j<level-v-v-1;j++){
			t = buf[level*v + v+j];
			buf[level*v + v+j] = buf[level*(v+j) + level-1-v];
			buf[level*(v+j) + level-1-v] = buf[level*(level-1-v) + level-1-v-j];
			buf[level*(level-1-v) + level-1-v-j] = buf[level*(level-1-v-j) + v];
			buf[level*(level-1-v-j) + v] = t;
		}
	}
}
static void rubikscube_rotate(u8* buf, int code)
{
	int j;
	u8 k[level];
	u8* l = &buf[level*level*0];
	u8* r = &buf[level*level*1];
	u8* n = &buf[level*level*2];
	u8* f = &buf[level*level*3];
	u8* b = &buf[level*level*4];
	u8* t = &buf[level*level*5];
	switch(code){
		case 'b':{
			rubikscube_rotateface(b);
			for(j=0;j<level;j++)k[j] = n[j];
			for(j=0;j<level;j++)n[j] = l[j];
			for(j=0;j<level;j++)l[j] = f[j];
			for(j=0;j<level;j++)f[j] = r[j];
			for(j=0;j<level;j++)r[j] = k[j];
			return;
		}
		case 't':{
			rubikscube_rotateface(t);
			for(j=0;j<level;j++)k[j] = n[level*(level-1) + j];
			for(j=0;j<level;j++)n[level*(level-1) + j] = r[level*(level-1) + j];
			for(j=0;j<level;j++)r[level*(level-1) + j] = f[level*(level-1) + j];
			for(j=0;j<level;j++)f[level*(level-1) + j] = l[level*(level-1) + j];
			for(j=0;j<level;j++)l[level*(level-1) + j] = k[j];
			return;
		}
		case 'l':{
			rubikscube_rotateface(l);
			for(j=0;j<level;j++)k[j] = b[j*level];
			for(j=0;j<level;j++)b[j*level] = n[j*level];
			for(j=0;j<level;j++)n[j*level] = t[j*level];
			for(j=0;j<level;j++)t[j*level] = f[(level-1-j)*level + level-1];
			for(j=0;j<level;j++)f[(level-1-j)*level + level-1] = k[j];
			return;
		}
		case 'r':{
			rubikscube_rotateface(r);
			for(j=0;j<level;j++)k[j] = t[level*j + level-1];
			for(j=0;j<level;j++)t[level*j + level-1] = n[level*j + level-1];
			for(j=0;j<level;j++)n[level*j + level-1] = b[level*j + level-1];
			for(j=0;j<level;j++)b[level*j + level-1] = f[level*(level-1-j)];
			for(j=0;j<level;j++)f[level*(level-1-j)] = k[j];
			return;
		}
		case 'n':{
			rubikscube_rotateface(n);
			for(j=0;j<level;j++)k[j] = r[level*j];
			for(j=0;j<level;j++)r[level*j] = t[level-1-j];
			for(j=0;j<level;j++)t[level-1-j] = l[level*(level-1-j) + level-1];
			for(j=0;j<level;j++)l[level*(level-1-j) + level-1] = b[level*(level-1) + j];
			for(j=0;j<level;j++)b[level*(level-1) + j] = k[j];
			return;
		}
		case 'f':{
			rubikscube_rotateface(f);
			for(j=0;j<level;j++)k[j] = l[level*j];
			for(j=0;j<level;j++)l[level*j] = t[level*(level-1) + j];
			for(j=0;j<level;j++)t[level*(level-1) + j] = r[level*(level-1-j) + level-1];
			for(j=0;j<level;j++)r[level*(level-1-j) + level-1] = b[level-1-j];
			for(j=0;j<level;j++)b[level-1-j] = k[j];
			return;
		}
	}
}
static int rubikscube_shouldrotate(int x, int y, int face, int code)
{
	switch(code){
	case 'l':{
		if('l' == face)return 1;
		if(('b' == face)&(x < 1))return 1;
		if(('t' == face)&(x < 1))return 1;
		if(('n' == face)&(x < 1))return 1;
		if(('f' == face)&(x > level-2))return 1;
		return 0;
	}
	case 'r':{
		if('r' == face)return 1;
		if(('b' == face)&(x > level-2))return 1;
		if(('t' == face)&(x > level-2))return 1;
		if(('n' == face)&(x > level-2))return 1;
		if(('f' == face)&(x < 1))return 1;
		return 0;
	}
	case 'n':{
		if('n' == face)return 1;
		if(('b' == face)&(y > level-2))return 1;
		if(('t' == face)&(y < 1))return 1;
		if(('l' == face)&(x > level-2))return 1;
		if(('r' == face)&(x < 1))return 1;
		return 0;
	}
	case 'f':{
		if('f' == face)return 1;
		if(('b' == face)&(y < 1))return 1;
		if(('t' == face)&(y > level-2))return 1;
		if(('l' == face)&(x < 1))return 1;
		if(('r' == face)&(x > level-2))return 1;
		return 0;
	}
	case 'b':{
		if('b' == face)return 1;
		if(('l' == face)&(y < 1))return 1;
		if(('r' == face)&(y < 1))return 1;
		if(('n' == face)&(y < 1))return 1;
		if(('f' == face)&(y < 1))return 1;
		return 0;
	}
	case 't':{
		if('t' == face)return 1;
		if(('l' == face)&(y > level-2))return 1;
		if(('r' == face)&(y > level-2))return 1;
		if(('n' == face)&(y > level-2))return 1;
		if(('f' == face)&(y > level-2))return 1;
		return 0;
	}
	}
	return 0;
}




static void rubikscube_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u32 bg;
	int x, y, cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	u8 (*buf)[4][4] = act->buf0;
	if(0 == buf)return;

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//left
			bg = rubikcolor[(buf[0][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);

			//right
			bg = rubikcolor[(buf[1][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+4*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+4*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//near
			bg = rubikcolor[(buf[2][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);

			//far
			bg = rubikcolor[(buf[3][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+6*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+6*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//bottom
			bg = rubikcolor[(buf[4][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+4*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+4*level)*hh/4/level
			);

			//upper
			bg = rubikcolor[(buf[5][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2)*hh/4/level
			);
		}
	}
}
static void rubikscube_draw_dx11(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int x,y,rgb;
	float c,s;
	vec4 f,q;
	vec3 tc, tr, tf, tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	u8 (*buf)[4][4] = act->buf0;
	if(0 == buf)return;

	if(act->CODE){
		u64 time = timeread();
		q[3] = (time - act->TIME) / 1000000.0;
		if((q[3] < 0.0)|(q[3] > 1.0)){
			rubikscube_rotate((void*)buf, act->CODE);
			act->CODE = 0;
		}

		if(act->CODE){
			q[0] = q[1] = q[2] = 0.0;
			switch(act->CODE){
				case 'l':q[0] =-1.0;break;
				case 'r':q[0] = 1.0;break;
				case 'n':q[1] =-1.0;break;
				case 'f':q[1] = 1.0;break;
				case 'b':q[2] =-1.0;break;
				case 't':q[2] = 1.0;break;
			}

			q[3] *= -PI/4;
			c = getcos(q[3]);
			s = getsin(q[3]);

			q[0] = q[0]*s;
			q[1] = q[1]*s;
			q[2] = q[2]*s;
			q[3] = c;
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//left
			tr[0] = -vf[0] / (level+0.5);
			tr[1] = -vf[1] / (level+0.5);
			tr[2] = -vf[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = -1.0;
			f[1] = 1.0 - (2.0*x+1.0)/level;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'l', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[0][y][x])%6];
			dx11solid_rect(ctx, rgb, tc, tr, tf);

			//right
			tr[0] = vf[0] / (level+0.5);
			tr[1] = vf[1] / (level+0.5);
			tr[2] = vf[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = 1.0;
			f[1] = (2.0*x+1.0)/level - 1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'r', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[1][y][x])%6];
			dx11solid_rect(ctx, rgb, tc, tr, tf);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//near
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = -1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'n', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[2][y][x])%6];
			dx11solid_rect(ctx, rgb, tc, tr, tf);

			//far
			tr[0] = -vr[0] / (level+0.5);
			tr[1] = -vr[1] / (level+0.5);
			tr[2] = -vr[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = 1.0 - (2.0*x+1.0)/level;
			f[1] = 1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'f', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[3][y][x])%6];
			dx11solid_rect(ctx, rgb, tc, tr, tf);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//bottom
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = -vf[0] / (level+0.5);
			tf[1] = -vf[1] / (level+0.5);
			tf[2] = -vf[2] / (level+0.5);
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = 1.0 - (2.0*y+1.0)/level;
			f[2] = -1.0;
			if(rubikscube_shouldrotate(x, y, 'b', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[4][y][x])%6];
			dx11solid_rect(ctx, rgb, tc, tr, tf);

			//upper
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = vf[0] / (level+0.5);
			tf[1] = vf[1] / (level+0.5);
			tf[2] = vf[2] / (level+0.5);
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = (2.0*y+1.0)/level - 1.0;
			f[2] = 1.0;
			if(rubikscube_shouldrotate(x, y, 't', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[5][y][x])%6];
			dx11solid_rect(ctx, rgb, tc, tr, tf);
		}
	}
}
static void rubikscube_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int x,y,rgb;
	float c,s;
	vec4 f,q;
	vec3 tc, tr, tf, tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	u8 (*buf)[4][4] = act->buf0;
	if(0 == buf)return;

	if(act->CODE){
		u64 time = timeread();
		q[3] = (time - act->TIME) / 1000000.0;
		if((q[3] < 0.0)|(q[3] > 1.0)){
			rubikscube_rotate((void*)buf, act->CODE);
			act->CODE = 0;
		}

		if(act->CODE){
			q[0] = q[1] = q[2] = 0.0;
			switch(act->CODE){
				case 'l':q[0] =-1.0;break;
				case 'r':q[0] = 1.0;break;
				case 'n':q[1] =-1.0;break;
				case 'f':q[1] = 1.0;break;
				case 'b':q[2] =-1.0;break;
				case 't':q[2] = 1.0;break;
			}

			q[3] *= -PI/4;
			c = getcos(q[3]);
			s = getsin(q[3]);

			q[0] = q[0]*s;
			q[1] = q[1]*s;
			q[2] = q[2]*s;
			q[3] = c;
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//left
			tr[0] = -vf[0] / (level+0.5);
			tr[1] = -vf[1] / (level+0.5);
			tr[2] = -vf[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = -1.0;
			f[1] = 1.0 - (2.0*x+1.0)/level;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'l', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[0][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);

			//right
			tr[0] = vf[0] / (level+0.5);
			tr[1] = vf[1] / (level+0.5);
			tr[2] = vf[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = 1.0;
			f[1] = (2.0*x+1.0)/level - 1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'r', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[1][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//near
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = -1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'n', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[2][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);

			//far
			tr[0] = -vr[0] / (level+0.5);
			tr[1] = -vr[1] / (level+0.5);
			tr[2] = -vr[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = 1.0 - (2.0*x+1.0)/level;
			f[1] = 1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'f', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[3][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//bottom
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = -vf[0] / (level+0.5);
			tf[1] = -vf[1] / (level+0.5);
			tf[2] = -vf[2] / (level+0.5);
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = 1.0 - (2.0*y+1.0)/level;
			f[2] = -1.0;
			if(rubikscube_shouldrotate(x, y, 'b', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[4][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);

			//upper
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = vf[0] / (level+0.5);
			tf[1] = vf[1] / (level+0.5);
			tf[2] = vf[2] / (level+0.5);
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = (2.0*y+1.0)/level - 1.0;
			f[2] = 1.0;
			if(rubikscube_shouldrotate(x, y, 't', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[5][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);
		}
	}
}
static void rubikscube_draw_mt20(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int x,y,rgb;
	float c,s;
	vec4 f,q;
	vec3 tc, tr, tf, tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	u8 (*buf)[4][4] = act->buf0;
	if(0 == buf)return;

	if(act->CODE){
		u64 time = timeread();
		q[3] = (time - act->TIME) / 1000000.0;
		if((q[3] < 0.0)|(q[3] > 1.0)){
			rubikscube_rotate((void*)buf, act->CODE);
			act->CODE = 0;
		}

		if(act->CODE){
			q[0] = q[1] = q[2] = 0.0;
			switch(act->CODE){
				case 'l':q[0] =-1.0;break;
				case 'r':q[0] = 1.0;break;
				case 'n':q[1] =-1.0;break;
				case 'f':q[1] = 1.0;break;
				case 'b':q[2] =-1.0;break;
				case 't':q[2] = 1.0;break;
			}

			q[3] *= -PI/4;
			c = getcos(q[3]);
			s = getsin(q[3]);

			q[0] = q[0]*s;
			q[1] = q[1]*s;
			q[2] = q[2]*s;
			q[3] = c;
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//left
			tr[0] = -vf[0] / (level+0.5);
			tr[1] = -vf[1] / (level+0.5);
			tr[2] = -vf[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = -1.0;
			f[1] = 1.0 - (2.0*x+1.0)/level;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'l', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[0][y][x])%6];
			mt20solid_rect(ctx, rgb, tc, tr, tf);

			//right
			tr[0] = vf[0] / (level+0.5);
			tr[1] = vf[1] / (level+0.5);
			tr[2] = vf[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = 1.0;
			f[1] = (2.0*x+1.0)/level - 1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'r', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[1][y][x])%6];
			mt20solid_rect(ctx, rgb, tc, tr, tf);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//near
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = -1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'n', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[2][y][x])%6];
			mt20solid_rect(ctx, rgb, tc, tr, tf);

			//far
			tr[0] = -vr[0] / (level+0.5);
			tr[1] = -vr[1] / (level+0.5);
			tr[2] = -vr[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			f[0] = 1.0 - (2.0*x+1.0)/level;
			f[1] = 1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			if(rubikscube_shouldrotate(x, y, 'f', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[3][y][x])%6];
			mt20solid_rect(ctx, rgb, tc, tr, tf);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//bottom
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = -vf[0] / (level+0.5);
			tf[1] = -vf[1] / (level+0.5);
			tf[2] = -vf[2] / (level+0.5);
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = 1.0 - (2.0*y+1.0)/level;
			f[2] = -1.0;
			if(rubikscube_shouldrotate(x, y, 'b', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[4][y][x])%6];
			mt20solid_rect(ctx, rgb, tc, tr, tf);

			//upper
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = vf[0] / (level+0.5);
			tf[1] = vf[1] / (level+0.5);
			tf[2] = vf[2] / (level+0.5);
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = (2.0*y+1.0)/level - 1.0;
			f[2] = 1.0;
			if(rubikscube_shouldrotate(x, y, 't', act->CODE)){
				quaternion_rotate(f, q);
				quaternion_rotate(tr,q);
				quaternion_rotate(tf,q);
			}
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			rgb = rubikcolor[(buf[5][y][x])%6];
			mt20solid_rect(ctx, rgb, tc, tr, tf);
		}
	}
}
static void rubikscube_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rubikscube_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rubikscube_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rubikscube_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("rubik(%x,%x,%x)\n",win,act,sty);
}




static void rubikscube_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	if(ev->what == _kbd_)
	{
	}
	else if(ev->what == _char_)
	{
	}
}




static void rubikscube_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(0 == stack)return;
	if('v' != key)return;

	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->fmt){
	case _dx11full_:rubikscube_draw_dx11(ent,slot, wor,geom, wnd,area);break;
	case _gl41full_:rubikscube_draw_gl41(ent,slot, wor,geom, wnd,area);break;
	case _mt20full_:rubikscube_draw_mt20(ent,slot, wor,geom, wnd,area);break;
	}
}
static void rubikscube_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	u8* p = buf;
	ent->CODE = p[0];
	ent->TIME = timeread();
}
static void rubikscube_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void rubikscube_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void rubikscube_search(struct entity* act)
{
}
static void rubikscube_modify(struct entity* act)
{
}
static void rubikscube_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void rubikscube_create(struct entity* act, void* str)
{
	int ret;
	void* buf;
	if(0 == act)return;
//printmemory(str,4);

	//malloc
	buf = act->buf0 = memorycreate(6 * level * level, 0);
	if(0 == buf)return;

	//read
	ret = 0;
	if(str)ret = rubikscube_import(str, buf);
	if((0==str)|(ret<=0))rubikscube_generate(buf, level);
	for(ret=0;ret<6;ret++)printmemory(buf + level*level*ret, level*level);
}




void rubikscube_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('r', 'u', 'b', 'i', 'k', 0, 0, 0);

	p->oncreate = (void*)rubikscube_create;
	p->ondelete = (void*)rubikscube_delete;
	p->onsearch = (void*)rubikscube_search;
	p->onmodify = (void*)rubikscube_modify;

	p->onlinkup = (void*)rubikscube_linkup;
	p->ondiscon = (void*)rubikscube_discon;
	p->ontaking = (void*)rubikscube_taking;
	p->ongiving = (void*)rubikscube_giving;
}
