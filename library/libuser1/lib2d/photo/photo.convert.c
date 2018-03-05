#define u8 unsigned char




//yuv444
void yuv2rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3)
{
}




//yuv422
void yuyv2rgba(
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

			z = (y+y2)*w1 + x+x2;
			z = z*4;

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
void yyuyyv2rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3)
{
}




//yuv420
void yx2rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3)
{
}