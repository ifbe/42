double squareroot(double);




double complex_modulus(double real, double imag)
{
	return squareroot(real*real + imag*imag);
}
double complex_angle(double real, double imag)
{
	//return atan2(imag, real);
	return 0.0;
}




void complex_mul(
	double real1, double imag1,
	double real2, double imag2,
	double* r, double* i)
{
	*r = real1*real2 - imag1*imag2;
	*i = real1*imag2 + real2*imag1;
}
void complex_div(
	double real1, double imag1,
	double real2, double imag2,
	double* r, double* i)
{
	double haha = real2*real2 + imag2*imag2;
	*r = (real1*real2 + imag1*imag2)/haha;
	*i = (real2*imag1 - real1*imag2)/haha;
}
