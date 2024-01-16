void logtoall(char*,...);




void qrcode_background(char* dest,int side)
{
	int x,y;
        for(y=0;y<side;y++)
        {
                for(x=0;x<side;x++)
                {
                        dest[(y*side)+x]=1;	//white
                }
        }
}
void qrcode_positiondetection(char* dest,int side,int xxxx,int yyyy)
{
	int x,y;

        for(y=yyyy-3;y<=yyyy+3;y++)
        {
                for(x=xxxx-3;x<=xxxx+3;x++)
                {
                        dest[(y*side)+x]=0;	//black
                }
        }
	for(x=xxxx-2;x<=xxxx+2;x++)
	{
		dest[(yyyy-2)*side + x]=1;
		dest[(yyyy+2)*side + x]=1;
	}
	for(y=yyyy-2;y<=yyyy+2;y++)
	{
		dest[y*side + xxxx-2]=1;
		dest[y*side + xxxx+2]=1;
	}
}
void qrcode_alignmentpattern(char* dest,int side,int xxxx,int yyyy)
{
	int x,y;
	for(y=yyyy-2;y<=yyyy+2;y++)
	{
		for(x=xxxx-2;x<=xxxx+2;x++)
		{
			dest[(y*side)+x]=0;	//black
		}
	}
	for(y=yyyy-1;y<=yyyy+1;y++)
	{
		for(x=xxxx-1;x<=xxxx+1;x++)
		{
			dest[(y*side)+x]=1;	//white
		}
	}
	dest[yyyy*side+xxxx]=0;			//black
}
void qrcode_timingpattern(char* dest,int side)
{
	int x,y;
	for(x=8;x<side-8;x+=2)dest[6*side+x]=0;
	for(y=8;y<side-8;y+=2)dest[y*side+6]=0;
}
void qrcode_formatinfo(char* dest,int side,unsigned short info)
{
	//calculate
	info  = (info<<10) + 0xdc;
	info ^= 0x5412;
	info  = ~info;

	//print
	dest[0*side + 8] = dest[8*side + side - 1] = info&0x1;
	dest[1*side + 8] = dest[8*side + side - 2] = (info>>1)&0x1;
	dest[2*side + 8] = dest[8*side + side - 3] = (info>>2)&0x1;
	dest[3*side + 8] = dest[8*side + side - 4] = (info>>3)&0x1;
	dest[4*side + 8] = dest[8*side + side - 5] = (info>>4)&0x1;
	dest[5*side + 8] = dest[8*side + side - 6] = (info>>5)&0x1;

	dest[7*side + 8] = dest[8*side + side - 7] = (info>>6)&0x1;
	dest[8*side + 8] = dest[8*side + side - 8] = (info>>7)&0x1;
	dest[8*side + 7] = dest[(side-7)*side + 8] = (info>>8)&0x1;

	dest[8*side + 5] = dest[(side-6)*side + 8] = (info>>9)&0x1;
	dest[8*side + 4] = dest[(side-5)*side + 8] = (info>>10)&0x1;
	dest[8*side + 3] = dest[(side-4)*side + 8] = (info>>11)&0x1;
	dest[8*side + 2] = dest[(side-3)*side + 8] = (info>>12)&0x1;
	dest[8*side + 1] = dest[(side-2)*side + 8] = (info>>13)&0x1;
	dest[8*side + 0] = dest[(side-1)*side + 8] = (info>>14)&0x1;
}

static unsigned int qrcode_v[50] = {
	      0,       0,       0,       0,       0,       0,       0,  0x7c94,
	0x085bc, 0x09a99, 0x0a4d3, 0x0bbf6, 0x0c762, 0x0d847, 0x0e60d, 0x0f928,
	0x10b78, 0x1145d, 0x12a17, 0x13532, 0x149a6, 0x15683, 0x168c9, 0x177ec,
	0x18ec4, 0x191e1, 0x1afab, 0x1b08e, 0x1cc1a, 0x1d33f, 0x1ed75, 0x1f250,
	0x209d5, 0x216f0, 0x228ba, 0x2379f, 0x24b0b, 0x2542e, 0x26a64, 0x27541,
	0x28c69
};
void qrcode_versioninfo(char* dest,int side,int version)
{
	int x,y;
	if(version<7)return;

	version = qrcode_v[version];
	version = ~version;
	for(y=0;y<6;y++)
	{
		for(x=0;x<3;x++)
		{
			dest[y*side + x+side-11] = (version >> (y*3+x))&0x1;
			dest[(x+side-11)*side + y] = (version >> (y*3+x))&0x1;
		}
	}
}




//src: @string
//dest: @table[len][len]
//sidelen: 21, 25, 29, 33, 37, 41, 45......177
//correction: L(7%), M(15%), Q(25%), H(30%)
void qrcode_generate(char* src, char* dest, int side, int correction)
{
	int version,ret;
	version=(side-17)/4;
	ret=(side-17)%4;
	if( (version<1) | (version>40) | (ret!=0) )
	{
		logtoall("error@qrcode version\n");
		return;
	}

	//
	qrcode_background(dest,side);

	//position detection
	qrcode_positiondetection(dest,side,3,3);
	qrcode_positiondetection(dest,side,side-4,3);
	qrcode_positiondetection(dest,side,3,side-4);

	//alignment pattern
	if( (version>=7)&&(version<=10) )
	{
		qrcode_alignmentpattern(dest,side,24,6);
		qrcode_alignmentpattern(dest,side,6,24);
		qrcode_alignmentpattern(dest,side,24,24);
		qrcode_alignmentpattern(dest,side,24,42);
		qrcode_alignmentpattern(dest,side,42,24);
		qrcode_alignmentpattern(dest,side,42,42);
	}

	//timing pattern
	qrcode_timingpattern(dest,side);

	//format info
	qrcode_formatinfo(dest,side,0x10);

	//version info
	qrcode_versioninfo(dest,side,version);
}
