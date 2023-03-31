#include "libuser.h"


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
void yyyyuv_to_yuvx(
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
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
say("gbgbxrgrgx_to_rgba\n");
printmemory(srcbuf, 0x40);
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
			dst0[4*x + 0] = (va[1]+vc[1])/2;
			dst0[4*x + 1] = vb[1];
			dst0[4*x + 2] = (vb[0]+vb[2])/2;
			dst0[4*x + 4] = (va[1]+va[3]+vc[1]+vc[3])/4;
			dst0[4*x + 5] = (va[2]+vc[2]+vb[1]+vb[3])/4;
			dst0[4*x + 6] = vb[2];

			dst0[4*x + 8] = (va[3]+vc[3])/2;
			dst0[4*x + 9] = vb[3];
			dst0[4*x +10] = (vb[2]+vb[4])/2;
			dst0[4*x +12] = (va[3]+va[5]+vc[3]+vc[5])/4;
			dst0[4*x +13] = (va[4]+vc[4]+vb[3]+vb[5])/4;
			dst0[4*x +14] = vb[4];

			dst1[4*x + 0] = vc[1];
			dst1[4*x + 1] = (vb[1]+vd[1]+vc[0]+vc[2])/4;
			dst1[4*x + 2] = (vb[0]+vb[2]+vd[0]+vd[2])/4;
			dst1[4*x + 4] = (vc[1]+vc[3])/2;
			dst1[4*x + 5] = vc[2];
			dst1[4*x + 6] = (vb[2]+vd[2])/2;

			dst1[4*x + 8] = vc[3];
			dst1[4*x + 9] = (vb[3]+vd[3]+vc[2]+vc[4])/4;
			dst1[4*x +10] = (vb[2]+vb[4]+vd[2]+vd[4])/4;
			dst1[4*x +12] = (vc[3]+vc[5])/2;
			dst1[4*x +13] = vc[4];
			dst1[4*x +14] = (vb[4]+vd[4])/2;
		}
	}
}
void bgbgxgrgrx_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
say("bgbgxgrgrx_to_rgba\n");
printmemory(srcbuf, 0x40);
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
			dst0[4*x + 0] = (va[0]+va[2]+vc[0]+vc[2])/4;
			dst0[4*x + 1] = (va[1]+vc[1]+vb[0]+vb[2])/4;
			dst0[4*x + 2] = vb[1];
			dst0[4*x + 4] = (va[2]+vc[2])/2;
			dst0[4*x + 5] = vb[2];
			dst0[4*x + 6] = (vb[1]+vb[3])/2;

			dst0[4*x + 8] = (va[2]+va[4]+vc[2]+vc[4])/4;
			dst0[4*x + 9] = (va[3]+vc[3]+vb[2]+vb[4])/4;
			dst0[4*x +10] = vb[3];
			dst0[4*x +12] = (va[4]+vc[4])/2;
			dst0[4*x +13] = vb[4];
			dst0[4*x +14] = (vb[3]+vb[5])/2;

			dst1[4*x + 0] = (vc[0]+vc[2])/2;
			dst1[4*x + 1] = vc[1];
			dst1[4*x + 2] = (vb[1]+vd[1])/2;
			dst1[4*x + 4] = vc[2];
			dst1[4*x + 5] = (vc[1]+vc[3]+vb[2]+vd[2])/2;
			dst1[4*x + 6] = (vb[1]+vb[3]+vc[1]+vc[3])/2;

			dst1[4*x + 8] = (vc[2]+vc[4])/2;
			dst1[4*x + 9] = vc[3];
			dst1[4*x +10] = (vb[3]+vd[3])/2;
			dst1[4*x +12] = vc[4];
			dst1[4*x +13] = (vc[3]+vc[5]+vb[4]+vd[4])/2;
			dst1[4*x +14] = (vb[3]+vb[5]+vc[3]+vc[5])/2;
		}
	}
}