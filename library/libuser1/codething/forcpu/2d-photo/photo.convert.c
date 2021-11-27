#define u8 unsigned char
#define u16 unsigned short




void bggr_to_rgba(
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
		src = srcbuf + (y&0xfffe)*srcw;
		for(x=0;x<srcw;x+=2)
		{
			dst[4*x + 0] = src[x + 1 + srcw];
			dst[4*x + 1] = src[x + 1];
			dst[4*x + 2] = src[x + 0];

			dst[4*x + 4] = src[x + 1 + srcw];
			dst[4*x + 5] = src[x + 1];
			dst[4*x + 6] = src[x + 0];
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
	u8* src;
	for(y=0;y<srch;y++)
	{
		dst = dstbuf + (y*dstw*4);
		src = srcbuf + (y&0xfffe)*srcw;
		for(x=0;x<srcw;x+=2)
		{
			dst[4*x + 0] = src[x + 0];
			dst[4*x + 1] = src[x + 1];
			dst[4*x + 2] = src[x + 1 + srcw];

			dst[4*x + 4] = src[x + 0];
			dst[4*x + 5] = src[x + 1];
			dst[4*x + 6] = src[x + 1 + srcw];
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
	u16* src;
	for(y=0;y<srch;y++)
	{
		dst = dstbuf + (y*dstw*4);
		src = (u16*)(srcbuf + (y&0xfffe)*srcw*2);
		for(x=0;x<srcw;x+=2)
		{
			dst[4*x + 0] = (src[x + 1 + srcw]>>2)&0xff;
			dst[4*x + 1] = (src[x + 1]>>2)&0xff;
			dst[4*x + 2] = (src[x + 0]>>2)&0xff;

			dst[4*x + 4] = (src[x + 1 + srcw]>>2)&0xff;
			dst[4*x + 5] = (src[x + 1]>>2)&0xff;
			dst[4*x + 6] = (src[x + 0]>>2)&0xff;
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
	u16* src;
	for(y=0;y<srch;y++)
	{
		dst = dstbuf + (y*dstw*4);
		src = (u16*)(srcbuf + (y&0xfffe)*srcw*2);
		for(x=0;x<srcw;x+=2)
		{
			dst[4*x + 0] = (src[x + 0]>>2)&0xff;
			dst[4*x + 1] = (src[x + 1]>>2)&0xff;
			dst[4*x + 2] = (src[x + 1 + srcw]>>2)&0xff;

			dst[4*x + 4] = (src[x + 0]>>2)&0xff;
			dst[4*x + 5] = (src[x + 1]>>2)&0xff;
			dst[4*x + 6] = (src[x + 1 + srcw]>>2)&0xff;
		}
	}
}




//yuv444
void yuv_to_rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3)
{
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