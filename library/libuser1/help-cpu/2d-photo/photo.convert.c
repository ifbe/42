#include "libuser.h"




void fixrgbbyccm(int* r, int* g, int* b, float (*ccm)[3])
{
	float out[3];
	float rgb[3] = {(float)*r, (float)*g, (float)*b};
	out[0] = (ccm[0][0]*rgb[0] + ccm[0][1]*rgb[1] + ccm[0][2]*rgb[2]);
	out[1] = (ccm[1][0]*rgb[0] + ccm[1][1]*rgb[1] + ccm[1][2]*rgb[2]);
	out[2] = (ccm[2][0]*rgb[0] + ccm[2][1]*rgb[1] + ccm[2][2]*rgb[2]);
	if(out[0] < 0.0)out[0] = 0.0;
	if(out[0] > 255.0)out[0] = 255.0;
	if(out[1] < 0.0)out[1] = 0.0;
	if(out[1] > 255.0)out[1] = 255.0;
	if(out[2] < 0.0)out[2] = 0.0;
	if(out[2] > 255.0)out[2] = 255.0;
	*r = (int)out[0];
	*g = (int)out[1];
	*b = (int)out[2];
}
u8 clamprgbfromfloat(float val)
{
	if(val < 0.0)val = 0.0;
	if(val > 255.0)val = 255.0;
	return (u8)val;
}




void rgb_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	int x,y;
	u8* src;
	u8* dst;
	for(y=0;y<dsth;y++){
		src = srcbuf + y*srcw*3;
		dst = dstbuf + y*dstw*4;
		for(x=0;x<dstw;x++){
			dst[x*4+0] = src[x*3+0];
			dst[x*4+1] = src[x*3+1];
			dst[x*4+2] = src[x*3+2];
		}
	}
}
void bgr_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	int x,y;
	u8* src;
	u8* dst;
	for(y=0;y<dsth;y++){
		src = srcbuf + y*srcw*3;
		dst = dstbuf + y*dstw*4;
		for(x=0;x<dstw;x++){
			dst[x*4+0] = src[x*3+2];
			dst[x*4+1] = src[x*3+1];
			dst[x*4+2] = src[x*3+0];
		}
	}
}


void bggr_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
	//printmemory(srcbuf, 0x10);

	int x,y;
	int r,g,b;
	u8* dst;
	u8* src = srcbuf;
	for(y=2;y<srch-2;y+=2)
	{
		if(y >= dsth)break;

		dst = dstbuf + y*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
			if(x >= dstw)break;
/*
g r g r g r
b g B-g b g		//B@(x,y)
g r g-r g r
*/
			b = src[srcw*y + x];
			g = (
				src[srcw*(y+0) + x-1]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y-1) + x+0]
			+	src[srcw*(y+1) + x+0]
			)/4;
			r = (
				src[srcw*(y-1) + x-1]
			+	src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			)/4;

			dst[4*x + 0] = 255-255*(4*r+1);
			dst[4*x + 1] = 255-255*(4*g+1);
			dst[4*x + 2] = 255-255*(4*b+1);
/*
g r g r g r
b g b-G b g		//G@(x+1,y)
g r g-r g r
*/
			b = (
				src[srcw*y + x+0]
			+	src[srcw*y + x+2]
			)/2;
			g = src[srcw*y + x+1];
			r = (
				src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x+1]
			)/2;

			dst[4*x + 4] = 255-255*(4*r+1);
			dst[4*x + 5] = 255-255*(4*g+1);
			dst[4*x + 6] = 255-255*(4*b+1);
		}

		dst = dstbuf + (y+1)*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
			if(x >= dstw)break;
/*
b g b-g b g
g r G-r g r		//G@(x,y+1)
b g b g b g
*/
			b = (
				src[srcw*(y+0) + x]
			+	src[srcw*(y+2) + x]
			)/2;
			g = src[srcw*(y+1) + x];
			r = (
				src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			)/2;

			dst[4*x + 0] = 255-255*(4*r+1);
			dst[4*x + 1] = 255-255*(4*g+1);
			dst[4*x + 2] = 255-255*(4*b+1);
/*
b g b-g b g
g r g-R g r		//R@(x+1,y+1)
b g b g b g
*/
			b = (
				src[srcw*(y+0) + x+0]
			+	src[srcw*(y+0) + x+2]
			+	src[srcw*(y+2) + x+0]
			+	src[srcw*(y+2) + x+2]
			)/4;
			g = (
				src[srcw*(y+1) + x+0]
			+	src[srcw*(y+1) + x+2]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y+2) + x+1]
			)/4;
			r = src[srcw*(y+1) + x+1];

			dst[4*x + 4] = 255-255*(4*r+1);
			dst[4*x + 5] = 255-255*(4*g+1);
			dst[4*x + 6] = 255-255*(4*b+1);
		}
	}
}
void bggr10_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
	//printmemory(srcbuf, 0x10);

	int x,y;
	int r,g,b;
	u8* dst;
	u16* src = (u16*)srcbuf;
	for(y=2;y<srch-2;y+=2)
	{
		if(y >= dsth)break;

		dst = dstbuf + y*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
			if(x >= dstw)break;
/*
g r g r g r
b g B-g b g		//R@(x,y)
g r g-r g r
*/
			b = src[srcw*y + x];
			g = (
				src[srcw*(y+0) + x-1]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y-1) + x+0]
			+	src[srcw*(y+1) + x+0]
			) / 4;
			r = (
				src[srcw*(y-1) + x-1]
			+	src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			) / 4;

			dst[4*x + 0] = 255-255*(r+1);
			dst[4*x + 1] = 255-255*(g+1);
			dst[4*x + 2] = 255-255*(b+1);
/*
g r g r g r
b g b-G b g		//G@(x+1,y)
g r g-r g r
*/
			b = (
				src[srcw*y + x+0]
			+	src[srcw*y + x+2]
			) / 2;
			g = src[srcw*y + x+1];
			r = (
				src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x+1]
			) / 2;

			dst[4*x + 4] = 255-255*(r+1);
			dst[4*x + 5] = 255-255*(g+1);
			dst[4*x + 6] = 255-255*(b+1);
		}

		dst = dstbuf + (y+1)*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
			if(x >= dstw)break;
/*
b g b-g b g
g r G-r g r		//G@(x,y+1)
b g b g b g
*/
			b = (
				src[srcw*(y+0) + x]
			+	src[srcw*(y+2) + x]
			) / 2;
			g = src[srcw*(y+1) + x];
			r = (
				src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			) / 2;

			dst[4*x + 0] = 255-255*(r+1);
			dst[4*x + 1] = 255-255*(g+1);
			dst[4*x + 2] = 255-255*(b+1);
/*
b g b-g b g
g r g-R g r		//B@(x+1,y+1)
b g b g b g
*/
			b = (
				src[srcw*(y+0) + x+0]
			+	src[srcw*(y+0) + x+2]
			+	src[srcw*(y+2) + x+0]
			+	src[srcw*(y+2) + x+2]
			) / 4;
			g = (
				src[srcw*(y+1) + x+0]
			+	src[srcw*(y+1) + x+2]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y+2) + x+1]
			) / 4;
			r = src[srcw*(y+1) + x+1];

			dst[4*x + 4] = 255-255*(r+1);
			dst[4*x + 5] = 255-255*(g+1);
			dst[4*x + 6] = 255-255*(b+1);
		}
	}
}




void rggb_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
	//printmemory(srcbuf, 0x10);

	int x,y;
	int r,g,b;
	u8* dst;
	u8* src = srcbuf;
	for(y=2;y<srch-2;y+=2)
	{
		if(y >= dsth)break;

		dst = dstbuf + y*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
			if(x >= dstw)break;
/*
g b g b g b
r g R-g r g		//R@(x,y)
g b g-b g b
*/
			r = src[srcw*y + x];
			g = (
				src[srcw*(y+0) + x-1]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y-1) + x+0]
			+	src[srcw*(y+1) + x+0]
			)/4;
			b = (
				src[srcw*(y-1) + x-1]
			+	src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			)/4;

			dst[4*x + 0] = 255-255*(4*r+1);
			dst[4*x + 1] = 255-255*(4*g+1);
			dst[4*x + 2] = 255-255*(4*b+1);
/*
g b g b g b
r g r-G r g		//G@(x+1,y)
g b g-b g b
*/
			r = (
				src[srcw*y + x+0]
			+	src[srcw*y + x+2]
			)/2;
			g = src[srcw*y + x+1];
			b = (
				src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x+1]
			)/2;

			dst[4*x + 4] = 255-255*(4*r+1);
			dst[4*x + 5] = 255-255*(4*g+1);
			dst[4*x + 6] = 255-255*(4*b+1);
		}

		dst = dstbuf + (y+1)*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
			if(x >= dstw)break;
/*
r g r-g r g
g b G-b g b		//G@(x,y+1)
r g r g r g
*/
			r = (
				src[srcw*(y+0) + x]
			+	src[srcw*(y+2) + x]
			)/2;
			g = src[srcw*(y+1) + x];
			b = (
				src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			)/2;

			dst[4*x + 0] = 255-255*(4*r+1);
			dst[4*x + 1] = 255-255*(4*g+1);
			dst[4*x + 2] = 255-255*(4*b+1);
/*
r g r-g r g
g b g-B g b		//B@(x+1,y+1)
r g r g r g
*/
			r = (
				src[srcw*(y+0) + x+0]
			+	src[srcw*(y+0) + x+2]
			+	src[srcw*(y+2) + x+0]
			+	src[srcw*(y+2) + x+2]
			)/4;
			g = (
				src[srcw*(y+1) + x+0]
			+	src[srcw*(y+1) + x+2]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y+2) + x+1]
			)/4;
			b = src[srcw*(y+1) + x+1];

			dst[4*x + 4] = 255-255*(4*r+1);
			dst[4*x + 5] = 255-255*(4*g+1);
			dst[4*x + 6] = 255-255*(4*b+1);
		}
	}
}
void rggb10_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
	//printmemory(srcbuf, 0x10);

	int x,y;
	int r,g,b;
	u8* dst;
	u16* src = (u16*)srcbuf;
	for(y=2;y<srch-2;y+=2)
	{
		if(y >= dsth)break;

		dst = dstbuf + y*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
			if(x >= dstw)break;
/*
g b g b g b
r g R-g r g		//R@(x,y)
g b g-b g b
*/
			r = src[srcw*y + x];
			g = (
				src[srcw*(y+0) + x-1]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y-1) + x+0]
			+	src[srcw*(y+1) + x+0]
			) / 4;
			b = (
				src[srcw*(y-1) + x-1]
			+	src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			) / 4;

			dst[4*x + 0] = 255-255*(r+1);
			dst[4*x + 1] = 255-255*(g+1);
			dst[4*x + 2] = 255-255*(b+1);
/*
g b g b g b
r g r-G r g		//G@(x+1,y)
g b g-b g b
*/
			r = (
				src[srcw*y + x+0]
			+	src[srcw*y + x+2]
			) / 2;
			g = src[srcw*y + x+1];
			b = (
				src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x+1]
			) / 2;

			dst[4*x + 4] = 255-255*(r+1);
			dst[4*x + 5] = 255-255*(g+1);
			dst[4*x + 6] = 255-255*(b+1);
		}

		dst = dstbuf + (y+1)*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
			if(x >= dstw)break;
/*
r g r-g r g
g b G-b g b		//G@(x,y+1)
r g r g r g
*/
			r = (
				src[srcw*(y+0) + x]
			+	src[srcw*(y+2) + x]
			) / 2;
			g = src[srcw*(y+1) + x];
			b = (
				src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			) / 2;

			dst[4*x + 0] = 255-255*(r+1);
			dst[4*x + 1] = 255-255*(g+1);
			dst[4*x + 2] = 255-255*(b+1);
/*
r g r-g r g
g b g-B g b		//B@(x+1,y+1)
r g r g r g
*/
			r = (
				src[srcw*(y+0) + x+0]
			+	src[srcw*(y+0) + x+2]
			+	src[srcw*(y+2) + x+0]
			+	src[srcw*(y+2) + x+2]
			) / 4;
			g = (
				src[srcw*(y+1) + x+0]
			+	src[srcw*(y+1) + x+2]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y+2) + x+1]
			) / 4;
			b = src[srcw*(y+1) + x+1];

			dst[4*x + 4] = 255-255*(r+1);
			dst[4*x + 5] = 255-255*(g+1);
			dst[4*x + 6] = 255-255*(b+1);
		}
	}
}




//yuv444
void yuvx_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	int temp;
	int x, y;
	int Y, U, V;
	u8* src;
	u8* dst;
	for(y=0;y<srch;y++)
	{
		src = srcbuf + 4*(srcw*y);
		dst = dstbuf + 4*(dstw*y);
		for(x=0;x<srcw;x++)
		{
			Y = src[4*x+0];
			U = src[4*x+1];
			V = src[4*x+2];

			temp = Y + (U-128) + ( (104*(U-128) )>>8);
			if(temp > 255)temp = 255;
			if(temp < 0)temp = 0;
			dst[4*x + 0] = temp;

			temp = Y - (89*(V-128)>>8) - ( (183*(U-128) )>>8);
			if(temp > 255)temp = 255;
			if(temp < 0)temp = 0;
			dst[4*x + 1] = temp;

			temp = Y + (V-128) + ( (199*(V-128) )>>8);
			if(temp > 255)temp = 255;
			if(temp < 0)temp = 0;
			dst[4*x + 2] = temp;
		}
	}
}




//yuv422
void yuyv_to_rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3)
{
	int x, y, z, w, h;
	int Y, U, V;
	int temp;
	if(src==0|dst==0)return;

	w = w0;
	h = h0;
	if(w > x3-x2)w = x3-x2;
	if(h > y3-y2)h = y3-y2;
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			Y = src[2*(y*w0) + 2*x];
			U = src[2*(y*w0) + 4*(x>>1) + 1];
			V = src[2*(y*w0) + 4*(x>>1) + 3];

			z = (y+y2)*w1 + (x+x2)*4;

			temp = Y + (U-128) + ( (104*(U-128) )>>8);
			if(temp > 255)temp = 255;
			if(temp < 0)temp = 0;
			dst[z + 0] = temp;

			temp = Y - (89*(V-128)>>8) - ( (183*(U-128) )>>8);
			if(temp > 255)temp = 255;
			if(temp < 0)temp = 0;
			dst[z + 1] = temp;

			temp = Y + (V-128) + ( (199*(V-128) )>>8);
			if(temp > 255)temp = 255;
			if(temp < 0)temp = 0;
			dst[z + 2] = temp;
		}
	}
}




//yuv411
void yyuyyv_to_rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3)
{
}




//yuv420
void yx_to_rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3)
{
}





void yuyv_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	//say("yuyv_to_yuvx:srcw=%d,srch=%d,dstw=%d,dsth=%d\n",srcw,srch,dstw,dsth);
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
	//printmemory(srcbuf, 0x10);

	int x,y;
	u8* dst;
	u8* src;
	for(y=0;y<srch;y++)
	{
		dst = dstbuf + (y*dstw*4);
		src = srcbuf + (y*srcw*2);
		for(x=0;x<srcw;x+=2)		//if(macos)yuyv, else uyvy
		{
			dst[4*x + 0] = src[2*x + 0];
			dst[4*x + 1] = src[2*x + 1];
			dst[4*x + 2] = src[2*x + 3];

			dst[4*x + 4] = src[2*x + 2];
			dst[4*x + 5] = src[2*x + 1];
			dst[4*x + 6] = src[2*x + 3];
		}
	}
}
void uyvy_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
	//printmemory(srcbuf, 0x10);

	int x,y;
	u8* dst;
	u8* src;
	for(y=0;y<srch;y++)
	{
		dst = dstbuf + (y*dstw*4);
		src = srcbuf + (y*srcw*2);
		for(x=0;x<srcw;x+=2)		//if(macos)yuyv, else uyvy
		{
			dst[4*x + 0] = src[2*x + 1];
			dst[4*x + 1] = src[2*x + 0];
			dst[4*x + 2] = src[2*x + 2];

			dst[4*x + 4] = src[2*x + 3];
			dst[4*x + 5] = src[2*x + 0];
			dst[4*x + 6] = src[2*x + 2];
		}
	}
}



/*
yyyyyyyy
yyyyyyyy
uvuvuvuv
*/
void y4_uv_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
	//printmemory(srcbuf, 0x10);

	int x,y;
	u8* dst;
	u8* srcp0;
	u8* srcp1;
	for(y=0;y<dsth;y++)
	{
		dst = dstbuf + (y*dstw*4);
		srcp0 = srcbuf + (y*srcw);
		srcp1 = srcbuf + srcw*srch + srcw*(y/2);
		for(x=0;x<dstw;x++)
		{
			dst[4*x + 0] = srcp0[x];
			dst[4*x + 1] = srcp1[(x&0xfffe)+0];
			dst[4*x + 2] = srcp1[(x&0xfffe)+1];
		}
	}
}



void gbgbxrgrgx_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth,
	mat3 ccm)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
//say("gbgbxrgrgx_to_rgba\n");
//printmemory(srcbuf, 0x40);
	int newx = 0;
	int srcstride = srcw*5/4;
	int x,y;
	int r,g,b;
	u8* dst0;
	u8* dst1;
	u8* src = srcbuf;
	short va[6];
	short vb[6];
	short vc[6];
	short vd[6];
	for(y=2;y<dsth-2;y+=2)
	{
		if(y >= dsth)break;

		dst0 = dstbuf + (y+0)*dstw*4;
		dst1 = dstbuf + (y+1)*dstw*4;
		for(x=4;x<dstw-4;x+=4)
		{
			if(x >= dstw)break;

			newx = (x/4)*5 + (x%4);
			va[0] = src[srcstride*(y-1) + newx-1-1];
			va[1] = src[srcstride*(y-1) + newx];
			va[2] = src[srcstride*(y-1) + newx+1];
			va[3] = src[srcstride*(y-1) + newx+2];
			va[4] = src[srcstride*(y-1) + newx+3];
			va[5] = src[srcstride*(y-1) + newx+4+1];

			vb[0] = src[srcstride*(y+0) + newx-1-1];
			vb[1] = src[srcstride*(y+0) + newx];
			vb[2] = src[srcstride*(y+0) + newx+1];
			vb[3] = src[srcstride*(y+0) + newx+2];
			vb[4] = src[srcstride*(y+0) + newx+3];
			vb[5] = src[srcstride*(y+0) + newx+4+1];

			vc[0] = src[srcstride*(y+1) + newx-1-1];
			vc[1] = src[srcstride*(y+1) + newx];
			vc[2] = src[srcstride*(y+1) + newx+1];
			vc[3] = src[srcstride*(y+1) + newx+2];
			vc[4] = src[srcstride*(y+1) + newx+3];
			vc[5] = src[srcstride*(y+1) + newx+4+1];

			vd[0] = src[srcstride*(y+2) + newx-1-1];
			vd[1] = src[srcstride*(y+2) + newx];
			vd[2] = src[srcstride*(y+2) + newx+1];
			vd[3] = src[srcstride*(y+2) + newx+2];
			vd[4] = src[srcstride*(y+2) + newx+3];
			vd[5] = src[srcstride*(y+2) + newx+4+1];
/*
      0   1 2 3 4   5
r g r g ~ r g r g ~ r g r g ~	//va
g b g b ~ G B G B ~ g b g b ~	//vb
r g r g ~ R G R G ~ r g r g ~	//vc
g b g b ~ g b g b ~ g b g b ~	//vd
*/
			r = (va[1]+vc[1])/2;
			g = vb[1];
			b = (vb[0]+vb[2])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst0[4*x + 0] = r;
			dst0[4*x + 1] = g;
			dst0[4*x + 2] = b;

			r = (va[1]+va[3]+vc[1]+vc[3])/4;
			g = (va[2]+vc[2]+vb[1]+vb[3])/4;
			b = vb[2];
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst0[4*x + 4] = r;
			dst0[4*x + 5] = g;
			dst0[4*x + 6] = b;

			r = (va[3]+vc[3])/2;
			g = vb[3];
			b = (vb[2]+vb[4])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst0[4*x + 8] = r;
			dst0[4*x + 9] = g;
			dst0[4*x +10] = b;

			r = (va[3]+va[5]+vc[3]+vc[5])/4;
			g = (va[4]+vc[4]+vb[3]+vb[5])/4;
			b = vb[4];
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst0[4*x +12] = r;
			dst0[4*x +13] = g;
			dst0[4*x +14] = b;

			r = vc[1];
			g = (vb[1]+vd[1]+vc[0]+vc[2])/4;
			b = (vb[0]+vb[2]+vd[0]+vd[2])/4;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst1[4*x + 0] = r;
			dst1[4*x + 1] = g;
			dst1[4*x + 2] = b;

			r = (vc[1]+vc[3])/2;
			g = vc[2];
			b = (vb[2]+vd[2])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst1[4*x + 4] = r;
			dst1[4*x + 5] = g;
			dst1[4*x + 6] = b;

			r = vc[3];
			g = (vb[3]+vd[3]+vc[2]+vc[4])/4;
			b = (vb[2]+vb[4]+vd[2]+vd[4])/4;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst1[4*x + 8] = r;
			dst1[4*x + 9] = g;
			dst1[4*x +10] = b;

			r = (vc[3]+vc[5])/2;
			g = vc[4];
			b = (vb[4]+vd[4])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst1[4*x +12] = r;
			dst1[4*x +13] = g;
			dst1[4*x +14] = b;
		}
	}
}
void gbgbxrgrgx_to_y4_u_v(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth,
	mat3 ccm)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
//say("gbgbxrgrgx_to_y4_u_v:srcw=%d,srch=%d,dstw=%d,dsth=%d\n",srcw,srch,dstw,dsth);
//printmemory(srcbuf, 0x40);
	int newx = 0;
	int srcstride = srcw*5/4;
	int x,y;
	int r,g,b;
	int sumr,sumg,sumb;
	float sumy,sumu,sumv;
	u8* dsty0;
	u8* dsty1;
	u8* dstu;
	u8* dstv;
	short va[6];
	short vb[6];
	short vc[6];
	short vd[6];
	u8* src = srcbuf;
	for(y=2;y<dsth-2;y+=2)
	{
		if(y+2 >= srch)break;		//1080
		if(y+2 >= dsth)break;		//1088
		//say("y=%d\n",y);

		dsty0 = dstbuf + (y+0)*dstw;
		dsty1 = dstbuf + (y+1)*dstw;
		dstu = dstbuf + dstw*dsth + (y/2)*(dstw/2);
		dstv = dstbuf + dstw*dsth + dstw*dsth/4 + (y/2)*(dstw/2);
		for(x=4;x<dstw-4;x+=4)
		{
			if(x >= dstw)break;

			newx = (x/4)*5 + (x%4);
			va[0] = src[srcstride*(y-1) + newx-1-1];
			va[1] = src[srcstride*(y-1) + newx];
			va[2] = src[srcstride*(y-1) + newx+1];
			va[3] = src[srcstride*(y-1) + newx+2];
			va[4] = src[srcstride*(y-1) + newx+3];
			va[5] = src[srcstride*(y-1) + newx+4+1];

			vb[0] = src[srcstride*(y+0) + newx-1-1];
			vb[1] = src[srcstride*(y+0) + newx];
			vb[2] = src[srcstride*(y+0) + newx+1];
			vb[3] = src[srcstride*(y+0) + newx+2];
			vb[4] = src[srcstride*(y+0) + newx+3];
			vb[5] = src[srcstride*(y+0) + newx+4+1];

			vc[0] = src[srcstride*(y+1) + newx-1-1];
			vc[1] = src[srcstride*(y+1) + newx];
			vc[2] = src[srcstride*(y+1) + newx+1];
			vc[3] = src[srcstride*(y+1) + newx+2];
			vc[4] = src[srcstride*(y+1) + newx+3];
			vc[5] = src[srcstride*(y+1) + newx+4+1];

			vd[0] = src[srcstride*(y+2) + newx-1-1];
			vd[1] = src[srcstride*(y+2) + newx];
			vd[2] = src[srcstride*(y+2) + newx+1];
			vd[3] = src[srcstride*(y+2) + newx+2];
			vd[4] = src[srcstride*(y+2) + newx+3];
			vd[5] = src[srcstride*(y+2) + newx+4+1];
/*
      0   1 2 3 4   5
r g r g ~ r g r g ~ r g r g ~	//va
g b g b ~ G B G B ~ g b g b ~	//vb
r g r g ~ R G R G ~ r g r g ~	//vc
g b g b ~ g b g b ~ g b g b ~	//vd
*/
			//yyyy
			r = (va[1]+vc[1])/2;
			g = vb[1];
			b = (vb[0]+vb[2])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty0[x + 0] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr = r;
			sumg = g;
			sumb = b;

			r = (va[1]+va[3]+vc[1]+vc[3])/4;
			g = (va[2]+vc[2]+vb[1]+vb[3])/4;
			b = vb[2];
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty0[x + 1] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			r = vc[1];
			g = (vb[1]+vd[1]+vc[0]+vc[2])/4;
			b = (vb[0]+vb[2]+vd[0]+vd[2])/4;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty1[x + 0] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			r = (vc[1]+vc[3])/2;
			g = vc[2];
			b = (vb[2]+vd[2])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty1[x + 1] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			//u,v
			sumr /= 4;
			sumg /= 4;
			sumb /= 4;
			sumy = 0.2126*sumr+0.7152*sumg+0.0722*sumb;
			dstu[x/2] = clamprgbfromfloat(128+(sumb-sumy)/1.8556);
			dstv[x/2] = clamprgbfromfloat(128+(sumr-sumy)/1.5748);
			//say("sumb=%d,sumr=%d,sumy=%f\n",sumb,sumr,sumy);

			//yyyy
			r = (va[3]+vc[3])/2;
			g = vb[3];
			b = (vb[2]+vb[4])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty0[x + 2] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr = r;
			sumg = g;
			sumb = b;

			r = (va[3]+va[5]+vc[3]+vc[5])/4;
			g = (va[4]+vc[4]+vb[3]+vb[5])/4;
			b = vb[4];
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty0[x + 3] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			r = vc[3];
			g = (vb[3]+vd[3]+vc[2]+vc[4])/4;
			b = (vb[2]+vb[4]+vd[2]+vd[4])/4;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty1[x + 2] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			r = (vc[3]+vc[5])/2;
			g = vc[4];
			b = (vb[4]+vd[4])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty1[x + 3] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			//u,v
			sumr /= 4;
			sumg /= 4;
			sumb /= 4;
			sumy = clamprgbfromfloat(0.2126*sumr+0.7152*sumg+0.0722*sumb);
			dstu[x/2 + 1] = clamprgbfromfloat(128+(sumb-sumy)/1.8556);
			dstv[x/2 + 1] = clamprgbfromfloat(128+(sumr-sumy)/1.5748);
		}
		//say("yyyyy=%d\n",y);
	}
}




void bgbgxgrgrx_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth,
	mat3 ccm)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
//say("bgbgxgrgrx_to_rgba\n");
//printmemory(srcbuf, 0x40);
	int newx = 0;
	int srcstride = srcw*5/4;
	int x,y;
	int r,g,b;
	u8* dst0;
	u8* dst1;
	u8* src = srcbuf;
	short va[6];
	short vb[6];
	short vc[6];
	short vd[6];
	for(y=2;y<dsth-2;y+=2)
	{
		if(y >= dsth)break;

		dst0 = dstbuf + (y+0)*dstw*4;
		dst1 = dstbuf + (y+1)*dstw*4;
		for(x=4;x<dstw-4;x+=4)
		{
			if(x >= dstw)break;

			newx = (x/4)*5 + (x%4);
			va[0] = src[srcstride*(y-1) + newx-1-1];
			va[1] = src[srcstride*(y-1) + newx];
			va[2] = src[srcstride*(y-1) + newx+1];
			va[3] = src[srcstride*(y-1) + newx+2];
			va[4] = src[srcstride*(y-1) + newx+3];
			va[5] = src[srcstride*(y-1) + newx+4+1];

			vb[0] = src[srcstride*(y+0) + newx-1-1];
			vb[1] = src[srcstride*(y+0) + newx];
			vb[2] = src[srcstride*(y+0) + newx+1];
			vb[3] = src[srcstride*(y+0) + newx+2];
			vb[4] = src[srcstride*(y+0) + newx+3];
			vb[5] = src[srcstride*(y+0) + newx+4+1];

			vc[0] = src[srcstride*(y+1) + newx-1-1];
			vc[1] = src[srcstride*(y+1) + newx];
			vc[2] = src[srcstride*(y+1) + newx+1];
			vc[3] = src[srcstride*(y+1) + newx+2];
			vc[4] = src[srcstride*(y+1) + newx+3];
			vc[5] = src[srcstride*(y+1) + newx+4+1];

			vd[0] = src[srcstride*(y+2) + newx-1-1];
			vd[1] = src[srcstride*(y+2) + newx];
			vd[2] = src[srcstride*(y+2) + newx+1];
			vd[3] = src[srcstride*(y+2) + newx+2];
			vd[4] = src[srcstride*(y+2) + newx+3];
			vd[5] = src[srcstride*(y+2) + newx+4+1];
/*
      0   1 2 3 4   5
g r g r ~ g r g r ~ g r g r ~	//va
b g b g ~ B G B G ~ b g b g ~	//vb
g r g r ~ G R G R ~ g r g r ~	//vc
b g b g ~ b g b g ~ b g b g ~	//vd
*/
			r = (va[0]+va[2]+vc[0]+vc[2])/4;
			g = (va[1]+vc[1]+vb[0]+vb[2])/4;
			b = vb[1];
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst0[4*x + 0] = r;
			dst0[4*x + 1] = g;
			dst0[4*x + 2] = b;

			r = (va[2]+vc[2])/2;
			g = vb[2];
			b = (vb[1]+vb[3])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst0[4*x + 4] = r;
			dst0[4*x + 5] = g;
			dst0[4*x + 6] = b;

			r = (va[2]+va[4]+vc[2]+vc[4])/4;
			g = (va[3]+vc[3]+vb[2]+vb[4])/4;
			b = vb[3];
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst0[4*x + 8] = r;
			dst0[4*x + 9] = g;
			dst0[4*x +10] = b;

			r = (va[4]+vc[4])/2;
			g = vb[4];
			b = (vb[3]+vb[5])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst0[4*x +12] = r;
			dst0[4*x +13] = g;
			dst0[4*x +14] = b;

			r = (vc[0]+vc[2])/2;
			g = vc[1];
			b = (vb[1]+vd[1])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst1[4*x + 0] = r;
			dst1[4*x + 1] = g;
			dst1[4*x + 2] = b;

			r = vc[2];
			g = (vc[1]+vc[3]+vb[2]+vd[2])/2;
			b = (vb[1]+vb[3]+vc[1]+vc[3])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst1[4*x + 4] = r;
			dst1[4*x + 5] = g;
			dst1[4*x + 6] = b;

			r = (vc[2]+vc[4])/2;
			g = vc[3];
			b = (vb[3]+vd[3])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst1[4*x + 8] = r;
			dst1[4*x + 9] = g;
			dst1[4*x +10] = b;

			r = vc[4];
			g = (vc[3]+vc[5]+vb[4]+vd[4])/2;
			b = (vb[3]+vb[5]+vc[3]+vc[5])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dst1[4*x +12] = r;
			dst1[4*x +13] = g;
			dst1[4*x +14] = b;
		}
	}
}
void bgbgxgrgrx_to_y4_u_v(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth,
	mat3 ccm)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
//say("bgbgxgrgrx_to_y4_u_v:srcw=%d,srch=%d,dstw=%d,dsth=%d\n",srcw,srch,dstw,dsth);
//printmemory(srcbuf, 0x40);
	int newx = 0;
	int srcstride = srcw*5/4;
	int x,y;
	int r,g,b;
	int sumr,sumg,sumb;
	float sumy,sumu,sumv;
	u8* dsty0;
	u8* dsty1;
	u8* dstu;
	u8* dstv;
	short va[6];
	short vb[6];
	short vc[6];
	short vd[6];
	u8* src = srcbuf;
	for(y=2;y<dsth-2;y+=2)
	{
		if(y+2 >= srch)break;		//1080
		if(y+2 >= dsth)break;		//1088
		//say("y=%d\n",y);

		dsty0 = dstbuf + (y+0)*dstw;
		dsty1 = dstbuf + (y+1)*dstw;
		dstu = dstbuf + dstw*dsth + (y/2)*(dstw/2);
		dstv = dstbuf + dstw*dsth + dstw*dsth/4 + (y/2)*(dstw/2);
		for(x=4;x<dstw-4;x+=4)
		{
			if(x >= dstw)break;

			newx = (x/4)*5 + (x%4);
			va[0] = src[srcstride*(y-1) + newx-1-1];
			va[1] = src[srcstride*(y-1) + newx];
			va[2] = src[srcstride*(y-1) + newx+1];
			va[3] = src[srcstride*(y-1) + newx+2];
			va[4] = src[srcstride*(y-1) + newx+3];
			va[5] = src[srcstride*(y-1) + newx+4+1];

			vb[0] = src[srcstride*(y+0) + newx-1-1];
			vb[1] = src[srcstride*(y+0) + newx];
			vb[2] = src[srcstride*(y+0) + newx+1];
			vb[3] = src[srcstride*(y+0) + newx+2];
			vb[4] = src[srcstride*(y+0) + newx+3];
			vb[5] = src[srcstride*(y+0) + newx+4+1];

			vc[0] = src[srcstride*(y+1) + newx-1-1];
			vc[1] = src[srcstride*(y+1) + newx];
			vc[2] = src[srcstride*(y+1) + newx+1];
			vc[3] = src[srcstride*(y+1) + newx+2];
			vc[4] = src[srcstride*(y+1) + newx+3];
			vc[5] = src[srcstride*(y+1) + newx+4+1];

			vd[0] = src[srcstride*(y+2) + newx-1-1];
			vd[1] = src[srcstride*(y+2) + newx];
			vd[2] = src[srcstride*(y+2) + newx+1];
			vd[3] = src[srcstride*(y+2) + newx+2];
			vd[4] = src[srcstride*(y+2) + newx+3];
			vd[5] = src[srcstride*(y+2) + newx+4+1];
/*
      0   1 2 3 4   5
g r g r ~ g r g r ~ g r g r ~	//va
b g b g ~ B G B G ~ b g b g ~	//vb
g r g r ~ G R G R ~ g r g r ~	//vc
b g b g ~ b g b g ~ b g b g ~	//vd
*/
			//yyyy
			r = (va[0]+va[2]+vc[0]+vc[2])/4;
			g = (va[1]+vc[1]+vb[0]+vb[2])/4;
			b = vb[1];
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty0[x + 0] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr = r;
			sumg = g;
			sumb = b;

			r = (va[2]+vc[2])/2;
			g = vb[2];
			b = (vb[1]+vb[3])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty0[x + 1] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			r = (vc[0]+vc[2])/2;
			g = vc[1];
			b = (vb[1]+vd[1])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty1[x + 0] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			r = vc[2];
			g = (vc[1]+vc[3]+vb[2]+vd[2])/2;
			b = (vb[1]+vb[3]+vc[1]+vc[3])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty1[x + 1] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			//u,v
			sumr /= 4;
			sumg /= 4;
			sumb /= 4;
			sumy = 0.2126*sumr+0.7152*sumg+0.0722*sumb;
			dstu[x/2] = clamprgbfromfloat(128+(sumb-sumy)/1.8556);
			dstv[x/2] = clamprgbfromfloat(128+(sumr-sumy)/1.5748);
			//say("sumb=%d,sumr=%d,sumy=%f\n",sumb,sumr,sumy);

			//yyyy
			r = (va[2]+va[4]+vc[2]+vc[4])/4;
			g = (va[3]+vc[3]+vb[2]+vb[4])/4;
			b = vb[3];
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty0[x + 2] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr = r;
			sumg = g;
			sumb = b;

			r = (va[4]+vc[4])/2;
			g = vb[4];
			b = (vb[3]+vb[5])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty0[x + 3] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			r = (vc[2]+vc[4])/2;
			g = vc[3];
			b = (vb[3]+vd[3])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty1[x + 2] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			r = vc[4];
			g = (vc[3]+vc[5]+vb[4]+vd[4])/2;
			b = (vb[3]+vb[5]+vc[3]+vc[5])/2;
			if(ccm)fixrgbbyccm(&r,&g,&b,ccm);
			dsty1[x + 3] = clamprgbfromfloat(0.2126*r+0.7152*g+0.0722*b);
			sumr += r;
			sumg += g;
			sumb += b;

			//u,v
			sumr /= 4;
			sumg /= 4;
			sumb /= 4;
			sumy = clamprgbfromfloat(0.2126*sumr+0.7152*sumg+0.0722*sumb);
			dstu[x/2 + 1] = clamprgbfromfloat(128+(sumb-sumy)/1.8556);
			dstv[x/2 + 1] = clamprgbfromfloat(128+(sumr-sumy)/1.5748);
		}
		//say("yyyyy=%d\n",y);
	}
}
//1. BT.601标准[1]——标清数字电视（SDTV)
	//Y=0.299R+0.587G+0.114B
	//128+(sumb-sumy)/1.772;
	//128+(sumr-sumy)/1.402;
//2. BT.709标准[2]——高清数字电视（HDTV)
	//Y=0.2126R+0.7152G+0.0722B
	//128+(sumb-sumy)/1.8556;
	//128+(sumr-sumy)/1.5748;
//3. BT.2020标准[3]——超高清数字电视（UHDTV)
	//Y=0.2627R+0.6780G+0.0593B
	//128+(sumb-sumy)/1.8814;
	//128+(sumr-sumy)/1.4746;
