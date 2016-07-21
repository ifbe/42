void say(char*,...);




void qrcode_positiondetection(char* dest,int sidelength,int xxxx,int yyyy)
{
	int x,y;

        for(y=yyyy;y<yyyy+7;y++)
        {
                for(x=xxxx;x<xxxx+7;x++)
                {
                        dest[(y*sidelength)+x]=0;	//black
                }
        }
	for(x=xxxx+1;x<xxxx+6;x++)
	{
		dest[(yyyy+1)*sidelength + x]=1;
		dest[(yyyy+5)*sidelength + x]=1;
	}
	for(y=yyyy+1;y<yyyy+6;y++)
	{
		dest[y*sidelength + xxxx+1]=1;
		dest[y*sidelength + xxxx+5]=1;
	}
}
void qrcode_generate(char* src,char* dest,int sidelength)
{
	int x,y;
	if(sidelength<21)
	{
		say("qrcode requires size > 21\n");
		return;
	}

        for(y=0;y<sidelength;y++)
        {
                for(x=0;x<sidelength;x++)
                {
                        dest[(y*sidelength)+x]=1;	//white
                }
        }
	qrcode_positiondetection(dest,sidelength,0,0);
	qrcode_positiondetection(dest,sidelength,sidelength-7,0);
	qrcode_positiondetection(dest,sidelength,0,sidelength-7);
}
