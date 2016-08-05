#define DW unsigned long long
double squareroot(double);




DW incircle_center(DW a, DW b, DW c)
{
	double x1 = a&0xffff;
	double y1 = (a>>16)&0xffff;
	double x2 = b&0xffff;
	double y2 = (b>>16)&0xffff;
	double x3 = b&0xffff;
	double y3 = (b>>16)&0xffff;

	double a = squareroot( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
	double b = squareroot( (x1-x3)*(x1-x3) + (y1-y3)*(y1-y3) );
	double c = squareroot( (x2-x3)*(x2-x3) + (y2-y3)*(y2-y3) );
	double x = (a*x1 + b*x2 + c*x3)/(a+b+c);
	double y = (a*y1 + b*y2 + c*y3)/(a+b+c);

	return (DW)x + ((DW)y<<16);
}
double incircle_radius(DW a, DW b, DW c)
{
	double x1 = a&0xffff;
	double y1 = (a>>16)&0xffff;
	double x2 = b&0xffff;
	double y2 = (b>>16)&0xffff;
	double x3 = b&0xffff;
	double y3 = (b>>16)&0xffff;

	double a = squareroot( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
	double b = squareroot( (x1-x3)*(x1-x3) + (y1-y3)*(y1-y3) );
	double c = squareroot( (x2-x3)*(x2-x3) + (y2-y3)*(y2-y3) );
	double p = (a+b+c)/2;
	double S = squareroot( p*(p-a)*(p-b)*(p-c) );

	return 2*S/(a+b+c);
}
DW circumcircle_center(DW a, DW b, DW c)
{
	double x1 = a&0xffff;
	double y1 = (a>>16)&0xffff;
	double x2 = b&0xffff;
	double y2 = (b>>16)&0xffff;
	double x3 = b&0xffff;
	double y3 = (b>>16)&0xffff;

	double t1 = x1*x1 + y1*y1;
	double t2 = x2*x2 + y2*y2;
	double t3 = x3*x3 + y3*y3;
	double temp = x1*y2 + x2*y3+x3*y1 - x1*y3-x2*y1 - x3*y2;
	double x = (t2*y3 + t1*y2 + t3*y1 - t2*y1 - t3*y2 - t1*y3)/temp/2;
	double y = (t3*x2 + t2*x1 + t1*x3 - t1*x2 - t2*x3 - t3*x1)/temp/2;

	return (DW)x + ((DW)y<<16);
}
double circumcircle_radius(DW a, DW b, DW c)
{
	double x1 = a&0xffff;
	double y1 = (a>>16)&0xffff;
	double x2 = b&0xffff;
	double y2 = (b>>16)&0xffff;
	double x3 = b&0xffff;
	double y3 = (b>>16)&0xffff;

	double a = squareroot( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
	double b = squareroot( (x1-x3)*(x1-x3) + (y1-y3)*(y1-y3) );
	double c = squareroot( (x2-x3)*(x2-x3) + (y2-y3)*(y2-y3) );
	double p = (a+b+c)/2;
	double S = squareroot( p*(p-a)*(p-b)*(p-c) );

	return a*b*c/(4*S);
}
