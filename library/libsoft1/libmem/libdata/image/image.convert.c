#define u8 unsigned char




//yuv422
void yuyv2rgba(
	u8* src, int w1, int h1,
	u8* dst, int w2, int h2)
{
	int x, y, w, h;
	int Y, U, V;
	int temp;
	if(src==0|dst==0)return;

	w = w1<w2 ? w1:w2;
	h = h1<h2 ? h1:h2;
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			Y = src[2*(y*w1) + 2*x];
			U = src[2*(y*w1) + 4*(x>>1) + 1];
			V = src[2*(y*w1) + 4*(x>>1) + 3];

			temp = Y + (U-128) + ( (104*(U-128) )>>8);
			if(temp > 255)temp = 255;
			if(temp < 0)temp = 0;
			dst[4*(y*w2 + x) + 0] = temp;

			temp = Y - (89*(V-128)>>8) - ( (183*(U-128) )>>8);
			if(temp > 255)temp = 255;
			if(temp < 0)temp = 0;
			dst[4*(y*w2 + x) + 1] = temp;

			temp = Y + (V-128) + ( (199*(V-128) )>>8);
			if(temp > 255)temp = 255;
			if(temp < 0)temp = 0;
			dst[4*(y*w2 + x) + 2] = temp;
		}
	}
}




//yuv420
void yuv2rgba(
	u8* src, int w1, int h1,
	u8* dst, int w2, int h2, int sx, int sy)
{
}
