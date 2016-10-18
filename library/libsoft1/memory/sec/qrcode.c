void say(char*,...);




void qrcode_background(char* dest,int sidelength)
{
	int x,y;
        for(y=0;y<sidelength;y++)
        {
                for(x=0;x<sidelength;x++)
                {
                        dest[(y*sidelength)+x]=1;	//white
                }
        }
}
void qrcode_positiondetection(char* dest,int sidelength,int xxxx,int yyyy)
{
	int x,y;

        for(y=yyyy-3;y<=yyyy+3;y++)
        {
                for(x=xxxx-3;x<=xxxx+3;x++)
                {
                        dest[(y*sidelength)+x]=0;	//black
                }
        }
	for(x=xxxx-2;x<=xxxx+2;x++)
	{
		dest[(yyyy-2)*sidelength + x]=1;
		dest[(yyyy+2)*sidelength + x]=1;
	}
	for(y=yyyy-2;y<=yyyy+2;y++)
	{
		dest[y*sidelength + xxxx-2]=1;
		dest[y*sidelength + xxxx+2]=1;
	}
}
void qrcode_alignmentpattern(char* dest,int sidelength,int xxxx,int yyyy)
{
	int x,y;
	for(y=yyyy-2;y<=yyyy+2;y++)
	{
		for(x=xxxx-2;x<=xxxx+2;x++)
		{
			dest[(y*sidelength)+x]=0;	//black
		}
	}
	for(y=yyyy-1;y<=yyyy+1;y++)
	{
		for(x=xxxx-1;x<=xxxx+1;x++)
		{
			dest[(y*sidelength)+x]=1;	//white
		}
	}
	dest[yyyy*sidelength+xxxx]=0;			//black
}
void qrcode_timingpattern(char* dest,int sidelength)
{
	int x,y;
	for(x=8;x<sidelength-8;x+=2)dest[6*sidelength+x]=0;
	for(y=8;y<sidelength-8;y+=2)dest[y*sidelength+6]=0;
}
void qrcode_formatinformation(char* dest,int sidelength,unsigned short information)
{
	//calculate
	information  = (information<<10) + 0xdc;
	information ^= 0x5412;
	information  = ~information;

	//print
	dest[0*sidelength + 8] = dest[8*sidelength + sidelength - 1] = information&0x1;
	dest[1*sidelength + 8] = dest[8*sidelength + sidelength - 2] = (information>>1)&0x1;
	dest[2*sidelength + 8] = dest[8*sidelength + sidelength - 3] = (information>>2)&0x1;
	dest[3*sidelength + 8] = dest[8*sidelength + sidelength - 4] = (information>>3)&0x1;
	dest[4*sidelength + 8] = dest[8*sidelength + sidelength - 5] = (information>>4)&0x1;
	dest[5*sidelength + 8] = dest[8*sidelength + sidelength - 6] = (information>>5)&0x1;

	dest[7*sidelength + 8] = dest[8*sidelength + sidelength - 7] = (information>>6)&0x1;
	dest[8*sidelength + 8] = dest[8*sidelength + sidelength - 8] = (information>>7)&0x1;
	dest[8*sidelength + 7] = dest[(sidelength-7)*sidelength + 8] = (information>>8)&0x1;

	dest[8*sidelength + 5] = dest[(sidelength-6)*sidelength + 8] = (information>>9)&0x1;
	dest[8*sidelength + 4] = dest[(sidelength-5)*sidelength + 8] = (information>>10)&0x1;
	dest[8*sidelength + 3] = dest[(sidelength-4)*sidelength + 8] = (information>>11)&0x1;
	dest[8*sidelength + 2] = dest[(sidelength-3)*sidelength + 8] = (information>>12)&0x1;
	dest[8*sidelength + 1] = dest[(sidelength-2)*sidelength + 8] = (information>>13)&0x1;
	dest[8*sidelength + 0] = dest[(sidelength-1)*sidelength + 8] = (information>>14)&0x1;
}

static unsigned int qrcode_v[50] = {
	      0,       0,       0,       0,       0,       0,       0,  0x7c94,
	0x085bc, 0x09a99, 0x0a4d3, 0x0bbf6, 0x0c762, 0x0d847, 0x0e60d, 0x0f928,
	0x10b78, 0x1145d, 0x12a17, 0x13532, 0x149a6, 0x15683, 0x168c9, 0x177ec,
	0x18ec4, 0x191e1, 0x1afab, 0x1b08e, 0x1cc1a, 0x1d33f, 0x1ed75, 0x1f250,
	0x209d5, 0x216f0, 0x228ba, 0x2379f, 0x24b0b, 0x2542e, 0x26a64, 0x27541,
	0x28c69
};
void qrcode_versioninformation(char* dest,int sidelength,int version)
{
	int x,y;
	if(version<7)return;

	version = qrcode_v[version];
	version = ~version;
	for(y=0;y<6;y++)
	{
		for(x=0;x<3;x++)
		{
			dest[y*sidelength + x+sidelength-11] = (version >> (y*3+x))&0x1;
			dest[(x+sidelength-11)*sidelength + y] = (version >> (y*3+x))&0x1;
		}
	}
}




void qrcode_generate(char* src,char* dest,int sidelength)
{
	int version,ret;
	version=(sidelength-17)/4;
	ret=(sidelength-17)%4;
	if( (version<2) | (version>40) | (ret!=0) )
	{
		say("error@qrcode version\n");
		return;
	}

	//
	qrcode_background(dest,sidelength);

	//position detection
	qrcode_positiondetection(dest,sidelength,3,3);
	qrcode_positiondetection(dest,sidelength,sidelength-4,3);
	qrcode_positiondetection(dest,sidelength,3,sidelength-4);

	//alignment pattern
	if( (version>=7)&&(version<=10) )
	{
		qrcode_alignmentpattern(dest,sidelength,24,6);
		qrcode_alignmentpattern(dest,sidelength,6,24);
		qrcode_alignmentpattern(dest,sidelength,24,24);
		qrcode_alignmentpattern(dest,sidelength,24,42);
		qrcode_alignmentpattern(dest,sidelength,42,24);
		qrcode_alignmentpattern(dest,sidelength,42,42);
	}

	//timing pattern
	qrcode_timingpattern(dest,sidelength);

	//format information
	qrcode_formatinformation(dest,sidelength,0x10);

	//version information
	qrcode_versioninformation(dest,sidelength,version);
}
