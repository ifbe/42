#define QWORD unsigned long long
#define BYTE unsigned char
//(e^(-x*x/(2*sigma^2)))/(sigma*sqrt(2*pi))
void blur_gauss_x(QWORD size,char* addr)
{
	
}
void blur_gauss_y(QWORD size,char* addr)
{
	
}
void blur_gauss_xy(QWORD size,char* addr)
{
	blur_gauss_x(size,addr);
	blur_gauss_x(size,addr)
}
//(e^(-(x*x+y*y)/(2*sigma^2)))/(2*pi*sigma*sigma)
void blur_gauss_2(QWORD size,char* addr)
{
	
}
//均值
void blur_box(QWORD size,char* addr)
{
	
}