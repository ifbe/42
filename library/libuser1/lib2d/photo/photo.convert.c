#define u8 unsigned char




//yuv444
void yuv2rgba(u8* src, u8* dst,
	int w1, int h1, int sx1, int sy1, int dx1, int dy1,
	int w2, int h2, int sx2, int sy2, int dx2, int dy2)
{
}




//yuv422
void yuyv2rgba(u8* src, u8* dst,
	int w1, int h1, int sx1, int sy1, int dx1, int dy1,
	int w2, int h2, int sx2, int sy2, int dx2, int dy2)
{
	int x, y, z, w, h;
	int Y, U, V;
	int temp;
	if(src==0|dst==0)return;

	w = w1;
	h = h1;
	if(w > dx2-sx2)w = dx2-sx2;
	if(h > dy2-sy2)h = dy2-sy2;
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			Y = src[2*(y*w1) + 2*x];
			U = src[2*(y*w1) + 4*(x>>1) + 1];
			V = src[2*(y*w1) + 4*(x>>1) + 3];

			z = (y+sy2)*w2 + x+sx2;
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
void yyuyyv2rgba(u8* src, u8* dst,
	int w1, int h1, int sx1, int sy1, int dx1, int dy1,
	int w2, int h2, int sx2, int sy2, int dx2, int dy2)
{
}




//yuv420
void yx2rgba(u8* src, u8* dst,
	int w1, int h1, int sx1, int sy1, int dx1, int dy1,
	int w2, int h2, int sx2, int sy2, int dx2, int dy2)
{
}