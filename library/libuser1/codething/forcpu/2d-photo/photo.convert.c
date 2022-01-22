#include "libuser.h"




void bggr_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;
	//printmemory(srcbuf, 0x10);

	int x,y;
	u8* dst;
	u8* src = srcbuf;
	for(y=2;y<srch-2;y+=2)
	{
		dst = dstbuf + y*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
/*
g r g r g r
b g B-g b g		//B@(x,y)
g r g-r g r
*/
			dst[4*x + 2] = 
				src[srcw*y + x];
			dst[4*x + 1] = (
				src[srcw*(y+0) + x-1]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y-1) + x+0]
			+	src[srcw*(y+1) + x+0]
			)/4;
			dst[4*x + 0] = (
				src[srcw*(y-1) + x-1]
			+	src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			)/4;
/*
g r g r g r
b g b-G b g		//G@(x+1,y)
g r g-r g r
*/
			dst[4*x + 6] = (
				src[srcw*y + x+0]
			+	src[srcw*y + x+2]
			)/2;
			dst[4*x + 5] = 
				src[srcw*y + x+1];
			dst[4*x + 4] = (
				src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x+1]
			)/2;
		}

		dst = dstbuf + (y+1)*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
/*
b g b-g b g
g r G-r g r		//G@(x,y+1)
b g b g b g
*/
			dst[4*x + 2] = (
				src[srcw*(y+0) + x]
			+	src[srcw*(y+2) + x]
			)/2;
			dst[4*x + 1] = 
				src[srcw*(y+1) + x];
			dst[4*x + 0] = (
				src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			)/2;
/*
b g b-g b g
g r g-R g r		//R@(x+1,y+1)
b g b g b g
*/
			dst[4*x + 6] = (
				src[srcw*(y+0) + x+0]
			+	src[srcw*(y+0) + x+2]
			+	src[srcw*(y+2) + x+0]
			+	src[srcw*(y+2) + x+2]
			)/4;
			dst[4*x + 5] = (
				src[srcw*(y+1) + x+0]
			+	src[srcw*(y+1) + x+2]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y+2) + x+1]
			)/4;
			dst[4*x + 4] = 
				src[srcw*(y+1) + x+1];
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
	u8* dst;
	u16* src = (u16*)srcbuf;
	for(y=2;y<srch-2;y+=2)
	{
		dst = dstbuf + y*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
/*
g r g r g r
b g B-g b g		//R@(x,y)
g r g-r g r
*/
			dst[4*x + 2] = 
				src[srcw*y + x] / 4;
			dst[4*x + 1] = (
				src[srcw*(y+0) + x-1]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y-1) + x+0]
			+	src[srcw*(y+1) + x+0]
			) / 4 / 4;
			dst[4*x + 0] = (
				src[srcw*(y-1) + x-1]
			+	src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			) / 4 / 4;
/*
g r g r g r
b g b-G b g		//G@(x+1,y)
g r g-r g r
*/
			dst[4*x + 6] = (
				src[srcw*y + x+0]
			+	src[srcw*y + x+2]
			) / 2 / 4;
			dst[4*x + 5] = 
				src[srcw*y + x+1] / 4;
			dst[4*x + 4] = (
				src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x+1]
			) / 2 / 4;
		}

		dst = dstbuf + (y+1)*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
/*
b g b-g b g
g r G-r g r		//G@(x,y+1)
b g b g b g
*/
			dst[4*x + 2] = (
				src[srcw*(y+0) + x]
			+	src[srcw*(y+2) + x]
			) / 2 / 4;
			dst[4*x + 1] = 
				src[srcw*(y+1) + x] / 4;
			dst[4*x + 0] = (
				src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			) / 2 / 4;
/*
b g b-g b g
g r g-R g r		//B@(x+1,y+1)
b g b g b g
*/
			dst[4*x + 6] = (
				src[srcw*(y+0) + x+0]
			+	src[srcw*(y+0) + x+2]
			+	src[srcw*(y+2) + x+0]
			+	src[srcw*(y+2) + x+2]
			) / 4 / 4;
			dst[4*x + 5] = (
				src[srcw*(y+1) + x+0]
			+	src[srcw*(y+1) + x+2]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y+2) + x+1]
			) / 4 / 4;
			dst[4*x + 4] = 
				src[srcw*(y+1) + x+1] / 4;
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
	u8* dst;
	u8* src = srcbuf;
	for(y=2;y<srch-2;y+=2)
	{
		dst = dstbuf + y*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
/*
g b g b g b
r g R-g r g		//R@(x,y)
g b g-b g b
*/
			dst[4*x + 0] = 
				src[srcw*y + x];
			dst[4*x + 1] = (
				src[srcw*(y+0) + x-1]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y-1) + x+0]
			+	src[srcw*(y+1) + x+0]
			)/4;
			dst[4*x + 2] = (
				src[srcw*(y-1) + x-1]
			+	src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			)/4;
/*
g b g b g b
r g r-G r g		//G@(x+1,y)
g b g-b g b
*/
			dst[4*x + 4] = (
				src[srcw*y + x+0]
			+	src[srcw*y + x+2]
			)/2;
			dst[4*x + 5] = 
				src[srcw*y + x+1];
			dst[4*x + 6] = (
				src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x+1]
			)/2;
		}

		dst = dstbuf + (y+1)*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
/*
r g r-g r g
g b G-b g b		//G@(x,y+1)
r g r g r g
*/
			dst[4*x + 0] = (
				src[srcw*(y+0) + x]
			+	src[srcw*(y+2) + x]
			)/2;
			dst[4*x + 1] = 
				src[srcw*(y+1) + x];
			dst[4*x + 2] = (
				src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			)/2;
/*
r g r-g r g
g b g-B g b		//B@(x+1,y+1)
r g r g r g
*/
			dst[4*x + 4] = (
				src[srcw*(y+0) + x+0]
			+	src[srcw*(y+0) + x+2]
			+	src[srcw*(y+2) + x+0]
			+	src[srcw*(y+2) + x+2]
			)/4;
			dst[4*x + 5] = (
				src[srcw*(y+1) + x+0]
			+	src[srcw*(y+1) + x+2]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y+2) + x+1]
			)/4;
			dst[4*x + 6] = 
				src[srcw*(y+1) + x+1];
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
	u8* dst;
	u16* src = (u16*)srcbuf;
	for(y=2;y<srch-2;y+=2)
	{
		dst = dstbuf + y*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
/*
g b g b g b
r g R-g r g		//R@(x,y)
g b g-b g b
*/
			dst[4*x + 0] = 
				src[srcw*y + x] / 4;
			dst[4*x + 1] = (
				src[srcw*(y+0) + x-1]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y-1) + x+0]
			+	src[srcw*(y+1) + x+0]
			) / 4 / 4;
			dst[4*x + 2] = (
				src[srcw*(y-1) + x-1]
			+	src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			) / 4 / 4;
/*
g b g b g b
r g r-G r g		//G@(x+1,y)
g b g-b g b
*/
			dst[4*x + 4] = (
				src[srcw*y + x+0]
			+	src[srcw*y + x+2]
			) / 2 / 4;
			dst[4*x + 5] = 
				src[srcw*y + x+1] / 4;
			dst[4*x + 6] = (
				src[srcw*(y-1) + x+1]
			+	src[srcw*(y+1) + x+1]
			) / 2 / 4;
		}

		dst = dstbuf + (y+1)*dstw*4;
		for(x=2;x<srcw-2;x+=2)
		{
/*
r g r-g r g
g b G-b g b		//G@(x,y+1)
r g r g r g
*/
			dst[4*x + 0] = (
				src[srcw*(y+0) + x]
			+	src[srcw*(y+2) + x]
			) / 2 / 4;
			dst[4*x + 1] = 
				src[srcw*(y+1) + x] / 4;
			dst[4*x + 2] = (
				src[srcw*(y+1) + x-1]
			+	src[srcw*(y+1) + x+1]
			) / 2 / 4;
/*
r g r-g r g
g b g-B g b		//B@(x+1,y+1)
r g r g r g
*/
			dst[4*x + 4] = (
				src[srcw*(y+0) + x+0]
			+	src[srcw*(y+0) + x+2]
			+	src[srcw*(y+2) + x+0]
			+	src[srcw*(y+2) + x+2]
			) / 4 / 4;
			dst[4*x + 5] = (
				src[srcw*(y+1) + x+0]
			+	src[srcw*(y+1) + x+2]
			+	src[srcw*(y+0) + x+1]
			+	src[srcw*(y+2) + x+1]
			) / 4 / 4;
			dst[4*x + 6] = 
				src[srcw*(y+1) + x+1] / 4;
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
	for(y=0;y<srch;y++)
	{
		dst = dstbuf + (y*dstw*4);
		srcp0 = srcbuf + (y*srcw);
		srcp1 = srcbuf + srcw*srch + srcw*(y/2);
		for(x=0;x<srcw;x++)
		{
			dst[4*x + 0] = srcp0[x];
			dst[4*x + 1] = srcp1[(x&0xfffe)+0];
			dst[4*x + 2] = srcp1[(x&0xfffe)+1];
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
