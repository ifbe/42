#include<stdio.h>
#include<math.h>
double exponent(double);
double power(double,double);
double lg(double);
double ln(double);
double logarithm(double,double);
double squareroot(double);
double cosine(double);
double sine(double);
double tangent(double);
double arccosine(double);
double arcsine(double);
double arctangent(double);




void main()
{
	printf("exp:\n	%lf\n	%lf\n",
		exp(42),
		exponent(42)
	);

	printf("pow(42,0.42):\n	%lf\n	%lf\n",
		pow(42,0.42) , power(42,0.42)
	);

	printf("lg:\n	%lf\n	%lf\n",
		log10(42),
		lg(42)
	);

	printf("ln:\n	%lf\n	%lf\n",
		log(42),
		ln(42) );

	printf("log(42,0.42):\n	%lf\n	%lf\n",
		log(42)/log(0.42),
		logarithm(42,0.42)
	);

	printf("sqrt:\n	%lf\n	%lf\n",
		sqrt(42),
		squareroot(42)
	);

	printf("cos:\n	%lf\n	%lf\n",
		cos(42),
		cosine(42)
	);

	printf("sin:\n	%lf\n	%lf\n",
		sin(42),
		sine(42)
	);

	printf("tan:\n	%lf\n	%lf\n",
		tan(42),
		tangent(42)
	);

	printf("acos:\n	%lf\n	%lf\n",
		acos(0.42),
		arccosine(0.42)
	);

	printf("asin:\n	%lf\n	%lf\n",
		asin(0.42),
		arcsine(0.42)
	);

	printf("atan:\n	%lf\n	%lf\n",
		atan(0.42),
		arctangent(0.42)
	);
}
