#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//touch
static u64 pointenter[10];
static u64 pointleave[10];
static int pointcount=0;
static int pointmax=0;




//why, what, where, when
void motion_explain(u64* p)
{
	int m,n;
	int dx0,dy0;
	int dx1,dy1;
	int dx2,dy2;

	m = p[1]&0xffff;
	n = (p[0] >> 48) & 0x07;

	if(m == 0x4070)
	{
		//say("[%d]@@@@@@@@@@@\n",n);
		pointleave[n] = p[0];
	}//point move

	else if(m == 0x2b70)
	{
		//say("[%d]++++++++++\n",n);
		pointcount++;
		pointmax++;
		pointenter[n] = p[0];
	}//point sensed

	else if(m == 0x2d70)
	{
		//say("[%d]---------\n",n);

		//
		pointleave[n] = p[0];
/*
		say("(%llx,%llx)->(%llx,%llx)\n",
			(pointenter[n]&0xffff),
			((pointenter[n]>>16)&0xffff),
			(pointleave[n]&0xffff),
			((pointleave[n]>>16)&0xffff)
		);
*/
		//
		pointcount--;
		if(pointcount==0)
		{
			if(pointmax==1)
			{
				pointmax=0;
				dx0 = (pointleave[0]&0xffff) - (pointenter[0]&0xffff);
				dy0 = ((pointleave[0]>>16)&0xffff) - ((pointenter[0]>>16)&0xffff);

				if( (dy0>-256) && (dy0<256) )
				{
					if(dx0<-128)    //left
					{
						p[1] = 0x64626b;
						p[0] = 0x25;
					}
					else if(dx0>128)	//right
					{
						p[1] = 0x64626b;
						p[0] = 0x27;
					}
					else    //point
					{
						p[1] = 0x2d6d;
						p[0] = (p[0]&0xffffffff) + ((u64)1<<48);
					}
				}
				if( (dx0>-256) && (dx0<256) )
				{
					if(dy0<-128)    //up
					{
						p[1] = 0x64626b;
						p[0] = 0x26;
					}
					else if(dy0>128)	//down
					{
						p[1] = 0x64626b;
						p[0] = 0x28;
					}
					else    //point
					{
						p[1] = 0x2d6d;
						p[0] = (p[0]&0xffffffff) + ((u64)1<<48);
					}
				}
			}

			else if(pointmax==2)
			{
				pointmax=0;
				dx0 = (pointleave[0]&0xffff) - (pointenter[0]&0xffff);
				dy0 = ((pointleave[0]>>16)&0xffff) - ((pointenter[0]>>16)&0xffff);
				dx1 = (pointleave[1]&0xffff) - (pointenter[1]&0xffff);
				dy1 = ((pointleave[1]>>16)&0xffff) - ((pointenter[1]>>16)&0xffff);
			}

			else if(pointmax>=3)
			{
				pointmax=0;
				dx0 = (pointleave[0]&0xffff) - (pointenter[0]&0xffff);
				dy0 = ((pointleave[0]>>16)&0xffff) - ((pointenter[0]>>16)&0xffff);
				dx1 = (pointleave[1]&0xffff) - (pointenter[1]&0xffff);
				dy1 = ((pointleave[1]>>16)&0xffff) - ((pointenter[1]>>16)&0xffff);
				dx2 = (pointleave[2]&0xffff) - (pointenter[2]&0xffff);
				dy2 = ((pointleave[2]>>16)&0xffff) - ((pointenter[2]>>16)&0xffff);
				if( (dx0>-256)&&(dx0<256)&&(dx1>-256)&&(dx1<256)&&(dx2>-256)&&(dx2<256) )
				{
					if( (dy0 > 128)&&(dy1 > 128)&&(dy2 > 128) )
					{
						//worker[1].xyze1 ^= 1;
					}
					else if( (dy0 < -128)&&(dy1 < -128)&&(dy2 < -128) )
					{
						//worker[2].xyze1 ^= 1;
					}
					else
					{
						p[1] = 0x64626b;
						p[0] = 0x1b;
					}
				}//3touch
			}//max>=3
		}//last one
	}//point gone
}




static void motion_list()
{
}
static void motion_choose()
{
}
static void motion_read()
{
}
static void motion_write()
{
}
static void motion_start()
{
}
static void motion_stop()
{
}
void motion_create(void* w, u64* p)
{
	p[0]=0x776f6c66;	//type
	p[1]=0x6e6f69746f6d;	//id

	p[10]=(u64)motion_start;
	p[11]=(u64)motion_stop;
	p[12]=(u64)motion_list;
	p[13]=(u64)motion_choose;
	p[14]=(u64)motion_read;
	p[15]=(u64)motion_write;
}
void motion_delete()
{
}
