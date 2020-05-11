#include "libuser.h"
void barycentric(float* o, float* p, float* a, float* b, float* c);




/*
void raster_position(
vec4 gl_Position,	//o
vec3 v,			//i
uniform mat)		//uniform
{
	gl_Position = mat*v;
}




void raster_fragment(
vec4 FragColor,		//o
vec3 v,vec3 n,		//i
uniform tex)		//uniform
{
	FragColor = vec3(1.0, 1.0, 1.0, 1.0);
}




void raster_trigon(struct supply* wnd, mat4 mat, float* t0, float* t1, float* t2)
{
	//step0: shader
	vert = raster_position;
	frag = raster_fragment;

	//step1: xyzw conv
	vec4 v0,v1,v2;
	vert(v0, t0, mat);
	vert(v1, t1, mat);
	vert(v2, t2, mat);

	//step2: each pixel
	vec3 v;
	while(1){
		v = this pixel

		float* pdepth = depth + v.y*stride + v.x;
		float* pcolor = color + v.y*stride + v.x;
		if(*pdepth < v.z){
			*pdepth = v.z;
			frag(pcolor, v, 0);
		}

		//next pixel
	}
}
*/




//window, viewport
//vertshader, fragshader
//a(xyz),n(xyz), b(xyz),n(xyz), c(xyz),n(xyz)
//matrix_clip_from_local
void raster_getndc(vec4 o, vec4 v, mat4 m)
{
	float inv;
	o[3] = (m[3][0]*v[0] + m[3][1]*v[1] + m[3][2]*v[2] + m[3][3]);

	inv = 1.0 / o[3];
	o[0] = (m[0][0]*v[0] + m[0][1]*v[1] + m[0][2]*v[2] + m[0][3]) * inv;
	o[1] = (m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]*v[2] + m[1][3]) * inv;
	o[2] = (m[2][0]*v[0] + m[2][1]*v[1] + m[2][2]*v[2] + m[2][3]) * inv;
}
static u32 raster_getrgb(vec3 n)
{
	u32 r,g,b;
	float w = 0.5 / vec3_getlen(n);
	b = (u32)(256*(n[0]*w+0.5));
	g = (u32)(256*(n[1]*w+0.5));
	r = (u32)(256*(n[2]*w+0.5));
	return (r<<16) + (g<<8) + (b);
}
void rastersolid_triangle(
	struct entity* wnd, struct fstyle* rect,
	void* vs, void* fs,
	float* vbuf, int tcnt,
	mat4 mat)
{
	int stride = wnd->fbwidth>>2;
	u32* color = wnd->colorbuf;
	float* depth = wnd->depthbuf;
	if(0 == depth){depth = wnd->depthbuf = memorycreate(2048*2048*4, 0);}

	int px = wnd->width * rect->vc[0];
	int py = wnd->height* rect->vc[1];
	int qx = wnd->width * rect->vq[0] + px;
	int qy = wnd->height* rect->vq[1] + py;
	int cx = (px+qx)/2;
	int cy = (py+qy)/2;
	int dx = (qx-px)/2;
	int dy = (qy-py)/2;
	//say("%d,%d,%d,%d\n", cx,cy, dx,dy);

	float* f;
	float dep;
	int j, rgb,tmp;

	vec4 vec[3];
	vec3 uvw,xy,tb,tm,tt;
	int x,y, lx,rx, left,right;
	int b,m,t, botx,boty, midx,midy, topx,topy;
	for(y=0;y<wnd->height;y++){
		for(x=0;x<wnd->width;x++)depth[y*stride+x] = 1000.0;
	}
	for(j=0;j<tcnt;j++){
		f = &vbuf[6*3*j];
		raster_getndc(vec[0],      f, mat);
		raster_getndc(vec[1], &f[ 6], mat);
		raster_getndc(vec[2], &f[12], mat);

		if(vec[0][1] < vec[1][1]){
			if(vec[1][1] < vec[2][1]){b = 0;m = 1;t = 2;}
			else if(vec[2][1] < vec[0][1]){b = 2;m = 0;t = 1;}
			else{b = 0;m = 2;t = 1;}
		}
		else{
			if(vec[0][1] < vec[2][1]){b = 1;m = 0;t = 2;}
			else if(vec[2][1] < vec[1][1]){b = 2;m = 1;t = 0;}
			else{b = 1;m = 2;t = 0;}
		}

		topx = cx + dx*vec[b][0];
		topy = cy - dy*vec[b][1];
		tt[0] = topx;
		tt[1] = topy;
		midx = cx + dx*vec[m][0];
		midy = cy - dy*vec[m][1];
		tm[0] = midx;
		tm[1] = midy;
		botx = cx + dx*vec[t][0];
		boty = cy - dy*vec[t][1];
		tb[0] = botx;
		tb[1] = boty;
		//say("%d,%d, %d,%d, %d,%d\n", botx,boty, midx,midy, topx,topy);

		if(topy == boty)continue;
		left = botx + (topx-botx)*(midy-boty)/(topy-boty);
		if(midx > left){right = midx;}
		else{right = left;left = midx;}
		//say("%d,%d, %d,%d, %d,%d, %d,%d\n", botx,boty, midx,midy, topx,topy, left,right);

		//test
		//drawline(wnd,rgb, topx,topy, midx,midy);
		//drawline(wnd,rgb, botx,boty, midx,midy);
		//drawline(wnd,rgb, topx,topy, botx,boty);
		//drawline(wnd,rgb, midx,midy, valx,midy);

		rgb = raster_getrgb(&f[3]);
		//drawsolid_triangle(wnd, rgb, topx,topy, midx,midy, botx,boty);

		//bot half scan line
		if(boty < midy){
		for(y=boty;y<=midy;y++){
			lx = botx + (y-boty)*(left -botx)/(midy-boty);
			rx = botx + (y-boty)*(right-botx)/(midy-boty);
			for(x=lx;x<rx;x++){
				if( (x<px)| (x>qx)| (y<py)|(y>qy))continue;

				xy[0] = x;xy[1] = y;
				barycentric(uvw, xy, tb,tm,tt);

				//1/z = u/bot.z + v/mid.z + w/top.z
				dep = 1.0/(uvw[0]/vec[b][2] + uvw[1]/vec[m][2] + uvw[2]/vec[t][2]);
				if(dep > depth[y*stride+x])continue;

				depth[y*stride+x] = dep;
				color[y*stride+x] = rgb;
			}
		}//for
		}//if

		//top half scan line
		if(midy < topy){
		for(y=topy;y>midy;y--){
			lx = topx + (y-topy)*(left -topx)/(midy-topy);
			rx = topx + (y-topy)*(right-topx)/(midy-topy);
			for(x=lx;x<rx;x++){
				if( (x<px)| (x>qx)| (y<py)|(y>qy))continue;

				xy[0] = x;xy[1] = y;
				barycentric(uvw, xy, tb,tm,tt);

				//1/z = u/bot.z + v/mid.z + w/top.z
				dep = 1.0/(uvw[0]/vec[b][2] + uvw[1]/vec[m][2] + uvw[2]/vec[t][2]);
				if(dep > depth[y*stride+x])continue;

				depth[y*stride+x] = dep;
				color[y*stride+x] = rgb;
			}
		}//for
		}//if
	}
}