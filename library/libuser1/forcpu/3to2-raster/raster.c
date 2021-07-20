#include "libuser.h"
void barycentric(float* o, float* p, float* a, float* b, float* c);




//window, viewport
//vertshader, fragshader
//a(xyz),n(xyz), b(xyz),n(xyz), c(xyz),n(xyz)
//matrix_clip_from_local
static int raster_vert(vec4 olist[], vec3 ilist[], mat4 m)
{
	float* oo = olist[0];
	float* ov = olist[1];
	float* on = olist[2];
	float* iv = ilist[0];
	float* in = ilist[1];

	ov[0] = iv[0];ov[1] = iv[1];ov[2] = iv[2];
	on[0] = in[0];on[1] = in[1];on[2] = in[2];

	//gl_Position = m*v
	oo[0] = m[0][0]*iv[0] + m[0][1]*iv[1] + m[0][2]*iv[2] + m[0][3];
	oo[1] = m[1][0]*iv[0] + m[1][1]*iv[1] + m[1][2]*iv[2] + m[1][3];
	oo[2] = m[2][0]*iv[0] + m[2][1]*iv[1] + m[2][2]*iv[2] + m[2][3];
	oo[3] = m[3][0]*iv[0] + m[3][1]*iv[1] + m[3][2]*iv[2] + m[3][3];
	return 0;
}
static u32 raster_frag(vec4 olist[], vec4 ilist[], vec4 uni[])
{
	//float* v = ilist[0];
	float* n = ilist[1];

	float w = 0.5 / vec3_getlen(n);
	u32 b = (u32)(256*(n[0]*w+0.5));
	u32 g = (u32)(256*(n[1]*w+0.5));
	u32 r = (u32)(256*(n[2]*w+0.5));
	return (r<<16) + (g<<8) + (b);
}
static void raster_sort(float** bot,float** mid,float** top, float* va,float* vb,float* vc)
{
	if(va[1] < vb[1]){
		if(vc[1] > vb[1]){
			*bot = va;
			*mid = vb;
			*top = vc;
			return;
		}

		if(vc[1] < va[1]){
			*bot = vc;
			*mid = va;
			*top = vb;
			return;
		}

		*bot = va;
		*mid = vc;
		*top = vb;
		return;
	}
	else{		//vb < va
		if(vc[1] > va[1]){
			*bot = vb;
			*mid = va;
			*top = vc;
			return;
		}

		if(vc[1] < vb[1]){
			*bot = vc;
			*mid = vb;
			*top = va;
			return;
		}

		*bot = vb;
		*mid = vc;
		*top = va;
	}
}
void rastersolid_triangle(
	_obj* wnd, struct fstyle* rect,
	void* vs, void* fs,
	float* vbuf, int vfmt, int vw, int vh,
	mat4 mat, void* per)
{
	int stride = wnd->whdf.fbwidth>>2;
	u32* color = wnd->rgbanode.buf;
	float* depth = wnd->rgbanode.depthbuf;

	int px = wnd->whdf.width * rect->vc[0];
	int py = wnd->whdf.height* rect->vc[1];
	int qx = wnd->whdf.width * rect->vq[0] + px;
	int qy = wnd->whdf.height* rect->vq[1] + py;
	int cx = (px+qx)/2;
	int cy = (py+qy)/2;
	int dx = (qx-px)/2;
	int dy = (qy-py)/2;
	//say("%d,%d,%d,%d\n", cx,cy, dx,dy);

	float inv,dep;
	int j,k, rgb;
	int (*vert)(vec4 out[], void*, void*) = vs;
	u32 (*frag)(vec4 out[], vec4 in[], void*) = fs;
	if(0==vert)vert = (void*)raster_vert;
	if(0==frag)frag = (void*)raster_frag;

	//vs
	float*      vs_in;		//3_float_per_attr, x_attr_per_point
	vec4 vs_out[3][4];		//4_float_per_attr, 4_attr_per_tri, 3_tri_per_primi
	//sort
	float* bot;
	float* mid;
	float* top;
	int botx,boty, midx,midy, topx,topy;
	//fs
	vec4 fs_out[4],fs_in[4];
	vec3 uvw,fix,xy,tb,tm,tt;
	//
	int x,y, lx,rx, left,right;
	for(j=0;j<vh;j++){
		vs_in = &vbuf[vw*j];
		vert(vs_out[0],          vs_in, mat);
		inv = 1.0 / vs_out[0][0][3];
		vs_out[0][0][0] *= inv;
		vs_out[0][0][1] *= inv;
		vs_out[0][0][2] *= inv;
		vert(vs_out[1], &vs_in[vfmt  ], mat);
		inv = 1.0 / vs_out[1][0][3];
		vs_out[1][0][0] *= inv;
		vs_out[1][0][1] *= inv;
		vs_out[1][0][2] *= inv;
		vert(vs_out[2], &vs_in[vfmt*2], mat);
		inv = 1.0 / vs_out[2][0][3];
		vs_out[2][0][0] *= inv;
		vs_out[2][0][1] *= inv;
		vs_out[2][0][2] *= inv;
		raster_sort(&bot,&mid,&top, vs_out[0][0], vs_out[1][0], vs_out[2][0]);

		topx = cx + dx*bot[0];
		topy = cy - dy*bot[1];
		tt[0] = topx;
		tt[1] = topy;
		midx = cx + dx*mid[0];
		midy = cy - dy*mid[1];
		tm[0] = midx;
		tm[1] = midy;
		botx = cx + dx*top[0];
		boty = cy - dy*top[1];
		tb[0] = botx;
		tb[1] = boty;
		//say("%d,%d, %d,%d, %d,%d\n", botx,boty, midx,midy, topx,topy);

		if(topy == boty)continue;
		left = botx + (topx-botx)*(midy-boty)/(topy-boty);
		if(midx > left){right = midx;}
		else{right = left;left = midx;}
		//say("%d,%d, %d,%d, %d,%d, %d,%d\n", botx,boty, midx,midy, topx,topy, left,right);
/*
		//test
		drawline(wnd,0xff0000, topx,topy, left,midy);
		drawline(wnd,0xffff00, topx,topy,right,midy);
		drawline(wnd,0x0000ff, botx,boty, left,midy);
		drawline(wnd,0x00ffff, botx,boty,right,midy);
*/
		//bot half scan line
		if(boty < midy){
		for(y=boty;y<=midy;y++){
			lx = botx + (y-boty)*(left -botx)/(midy-boty);
			rx = botx + (y-boty)*(right-botx)/(midy-boty);
			for(x=lx;x<rx;x++){
				if( (x<px)| (x>=qx)| (y<py)|(y>=qy))continue;

				//ndc.bot = screen.top, ndc.top = screen.bot
				xy[0] = x;xy[1] = y;
				barycentric(uvw, xy, tt,tm,tb);

				//depth: 1/z = u/bot.z + v/mid.z + w/top.z
				dep = uvw[0]*bot[2] + uvw[1]*mid[2] + uvw[2]*top[2];
				if(dep > depth[y*stride+x])continue;
				depth[y*stride+x] = dep;

				//color: attr = a0*u/W + a1*v/W + a2*w/W;
				inv = 1.0 / (uvw[0]/bot[3] + uvw[1]/mid[3] + uvw[2]/top[3]);
				fix[0] = inv*uvw[0]/bot[3];
				fix[1] = inv*uvw[1]/mid[3];
				fix[2] = inv*uvw[2]/top[3];
				for(k=0;k<4;k++)fs_in[0][k] = fix[0]*bot[ 4+k] + fix[1]*mid[ 4+k] + fix[2]*top[ 4+k];
				for(k=0;k<4;k++)fs_in[1][k] = fix[0]*bot[ 8+k] + fix[1]*mid[ 8+k] + fix[2]*top[ 8+k];
				for(k=0;k<4;k++)fs_in[2][k] = fix[0]*bot[12+k] + fix[1]*mid[12+k] + fix[2]*top[12+k];
				color[y*stride+x] = frag(fs_out, fs_in, per);
			}
		}//for
		}//if

		//top half scan line
		if(midy < topy){
		for(y=topy;y>midy;y--){
			lx = topx + (y-topy)*(left -topx)/(midy-topy);
			rx = topx + (y-topy)*(right-topx)/(midy-topy);
			for(x=lx;x<rx;x++){
				if( (x<px)| (x>=qx)| (y<py)|(y>=qy) )continue;

				//ndc.bot = screen.top, ndc.top = screen.bot
				xy[0] = x;xy[1] = y;
				barycentric(uvw, xy, tt,tm,tb);

				//depth: 1/z = u/bot.z + v/mid.z + w/top.z
				dep = uvw[0]*bot[2] + uvw[1]*mid[2] + uvw[2]*top[2];
				if(dep > depth[y*stride+x])continue;
				depth[y*stride+x] = dep;

				//color:
				inv = 1.0 / (uvw[0]/bot[3] + uvw[1]/mid[3] + uvw[2]/top[3]);
				fix[0] = inv*uvw[0]/bot[3];
				fix[1] = inv*uvw[1]/mid[3];
				fix[2] = inv*uvw[2]/top[3];
				for(k=0;k<4;k++)fs_in[0][k] = fix[0]*bot[ 4+k] + fix[1]*mid[ 4+k] + fix[2]*top[ 4+k];
				for(k=0;k<4;k++)fs_in[1][k] = fix[0]*bot[ 8+k] + fix[1]*mid[ 8+k] + fix[2]*top[ 8+k];
				for(k=0;k<4;k++)fs_in[2][k] = fix[0]*bot[12+k] + fix[1]*mid[12+k] + fix[2]*top[12+k];
				color[y*stride+x] = frag(fs_out, fs_in, per);
			}
		}//for
		}//if

	}//per primitive
}