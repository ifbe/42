#define QWORD unsigned long long
#define BYTE unsigned char
//(e^(-x*x/(2*sigma^2)))/(sigma*sqrt(2*pi))
void blur_gauss_x(QWORD size,QWORD radius,char* src,char* dst)
{
	
}
void blur_gauss_y(QWORD size,QWORD radius,char* src,char* dst)
{
	
}
void blur_gauss_xy(QWORD size,QWORD radius,char* src,char* dst)
{
	blur_gauss_x(size,radius,src,dst);
	blur_gauss_x(size,radius,src,dst);
}
//(e^(-(x*x+y*y)/(2*sigma^2)))/(2*pi*sigma*sigma)
void blur_gauss_2(QWORD size,QWORD radius,char* src,char* dst)
{
	
}
//均值
void blur_box(QWORD size,QWORD radius,char* src,char* dst)
{
	
}
