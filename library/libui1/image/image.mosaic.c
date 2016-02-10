#define QWORD unsigned long long
#define BYTE unsigned char
void mosaic(QWORD size,QWORD partradius,BYTE* src,BYTE* dst)
{
	int width=size&0xffff;
	int height=(size>>16)&0xffff;

	int partx=size&0xffff;
	int party=(size>>16)&0xffff;
	int radius=(size>>48)&0xffff;
}
