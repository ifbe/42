#define u8 unsigned char




void scale_image(u8* src, u8* dst,
	int sw, int sh, int sx1, int sy1, int sx2, int sy2,
	int dw, int dh, int dx1, int dy1, int dx2, int dy2)
{
	int x,y;
	int r,g,b;
	int xa,xb,ya,yb;
	float fx,fy;

	fx = (float)(sx2-sx1) / (float)(dx2-dx1);
	fy = (float)(sy2-sy1) / (float)(dy2-dy1);
	for(y=dy1;y<dy2-1;y++)
	{
		for(x=dx1;x<dx2-1;x++)
		{
			xa = sx1 + (int)(fx*(x-dx1));
			xb = sx1 + (int)(fx*(x-dx1+1));
			ya = sy1 + (int)(fy*(y-dy1));
			yb = sy1 + (int)(fy*(y-dy1+1));

			if(xa>=sw)continue;
			if(xb>=sw)continue;
			if(ya>=sh)continue;
			if(yb>=sh)continue;

			b = src[(xa+(ya*sw))*4 + 0]
			  + src[(xb+(ya*sw))*4 + 0]
			  + src[(xa+(yb*sw))*4 + 0]
			  + src[(xb+(yb*sw))*4 + 0];
			b = b/4;
			if(b>255)b = 255;

			g = src[(xa+(ya*sw))*4 + 1]
			  + src[(xb+(ya*sw))*4 + 1]
			  + src[(xa+(yb*sw))*4 + 1]
			  + src[(xb+(yb*sw))*4 + 1];
			g = g/4;
			if(g>255)g = 255;

			r = src[(xa+(ya*sw))*4 + 2]
			  + src[(xb+(ya*sw))*4 + 2]
			  + src[(xa+(yb*sw))*4 + 2]
			  + src[(xb+(yb*sw))*4 + 2];
			r = r/4;
			if(r>255)r = 255;

			dst[(x+(y*dw))*4 + 0] = b;
			dst[(x+(y*dw))*4 + 1] = g;
			dst[(x+(y*dw))*4 + 2] = r;
			dst[(x+(y*dw))*4 + 3] = 0xff;
		}
	}
}
