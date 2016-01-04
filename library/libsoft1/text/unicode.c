int utf2unicode(unsigned char* src,unsigned int* dst)
{
	//[0,0x7f]:normal anscii
	if(src[0]<0x80)
	{
		dst[0]=src[0];
		return 1;
	}

	//(error)[0x80,0xbf]:part of one utf8
	if(src[0]<0xc0)
	{
		dst[0]=0x20;
		return 1;
	}

	//[0xc0,0xdf]:双字节字符
	if(src[0]<0xe0)
	{
		dst[0]	=((src[0]&0x1f)<<6)	//src[0:4,0:0]
			+ (src[1]&0x3f);	//src[1:5,1:0]
		return 2;
	}

	//[0xe0,0xef]:三字节字符
	if(src[0]<0xf0)
	{
		dst[0]	=((src[0]&0xf)<<12)	//src[0:3,0:0]
			+ ((src[1]&0x3f)<<6)	//src[1:5,1:0]
			+ (src[2]&0x3f);	//src[2:5,2:0]
		return 3;
	}

	//[0xf0,0xf7]:四字节字符
	if(src[0]<0xf8)
	{
		dst[0]	=((src[0]&0x7)<<18)	//src[0:2,0:0]
			+ ((src[1]&0x3f)<<12)	//src[1:5,1:0]
			+ ((src[2]&0x3f)<<6)	//src[2:5,2:0]
			+ (src[3]&0x3f);	//src[3:5,3:0]
		return 4;
	}

	//[0xf0,0xf7]:五字节字符
	if(src[0]<0xfc)
	{
		dst[0]	=((src[0]&0x3)<<24)	//src[0:1,0:0]
			+ ((src[1]&0x3f)<<18)	//src[1:5,1:0]
			+ ((src[2]&0x3f)<<12)	//src[2:5,2:0]
			+ ((src[3]&0x3f)<<6)	//src[3:5,3:0]
			+ (src[4]&0x3f);	//src[4:5,4:0]
		return 4;
	}

	//[0xf0,0xf7]:六字节字符
	if(src[0]<0xfe)
	{
		dst[0]	=((src[0]&0x1)<<30)	//src[0:0]
			+ ((src[1]&0x3f)<<24)	//src[1:5,1:0]
			+ ((src[2]&0x3f)<<18);	//src[2:5,2:0]
			+ ((src[3]&0x3f)<<12);	//src[3:5,3:0]
			+ ((src[4]&0x3f)<<6);	//src[4:5,4:0]
			+ (src[5]&0x3f);	//src[5:5,5:0]
		return 4;
	}

	return 0;
}
int unicode2utf(unsigned int src,unsigned char* dst)
{
	//[0,0x7f]:normal anscii
	if(src<0x80)
	{
		dst[0]=src;
		return 1;
	}

	//[0x80,0x7ff]:双字节字符
	if(src<0x800)
	{
		dst[0]=0xc0+(src>>6);
		dst[1]=0x80+(src&0x3f);
		return 2;
	}

	//[0x800,0xffff]:三字节字符
	if(src<0x10000)
	{
		dst[0]=0xe0+(src>>12);
		dst[1]=0x80+((src>>6)&0x3f);
		dst[2]=0x80+(src&0x3f);
		return 3;
	}

	//[0x10000,0x10ffff]:四字节字符
	if(src<0x200000)
	{
		dst[0]=0xe0+(src>>18);
		dst[1]=0x80+((src>>12)&0x3f);
		dst[2]=0x80+((src>>6)&0x3f);
		dst[3]=0x80+(src&0x3f);
		return 4;
	}

	//[0x200000,0x3ffffff]:五字节字符
	if(src<0x4000000)
	{
		dst[0]=0xe0+(src>>24);
		dst[1]=0x80+((src>>18)&0x3f);
		dst[2]=0x80+((src>>12)&0x3f);
		dst[3]=0x80+((src>>6)&0x3f);
		dst[4]=0x80+(src&0x3f);
		return 5;
	}

	//[0x4000000,0x7fffffff]:六字节字符
	if(src<0x80000000)
	{
		dst[0]=0xe0+(src>>30);
		dst[1]=0x80+((src>>24)&0x3f);
		dst[2]=0x80+((src>>18)&0x3f);
		dst[3]=0x80+((src>>12)&0x3f);
		dst[4]=0x80+((src>>6)&0x3f);
		dst[5]=0x80+(src&0x3f);
		return 6;
	}

	return 0;
}
