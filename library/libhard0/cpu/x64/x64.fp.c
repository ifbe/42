#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void say(char*,...);




//绝对值
double absolute(double x)
{
	register double result;
	__asm __volatile__
	(
		"fabs"
		:"=t" (result)
		:"0" (x)
	);
	return result;
}
/*
//取整
double floor(double x)
{
	register long double value;

	__volatile unsigned short int cw;
	__volatile unsigned short int cwtmp;
	__asm __volatile("fnstcw %0":"=m"(cw));
	cwtmp = (cw & 0xf3ff) | 0x0400;

	__asm __volatile("fldcw %0"::"m"(cwtmp));
	__asm __volatile("frndint":"=t"(value):"0"(x));
	__asm __volatile("fldcw %0"::"m"(cw));
	return value;
}
*/




//平方根
double squareroot(double x)
{
	register double result;
	__asm __volatile__
	(
		"fsqrt"
		:"=t"(result)
		:"0"(x)
	);
	return result;
}




//正弦
double sine(double x)
{
	register double result;
	__asm __volatile__
	(
		"fsin"
		:"=t"(result)
		:"0"(x)
	);
	return result;
}
//余弦
double cosine(double x)
{
	register double result;
	__asm __volatile__
	(
		"fcos"
		:"=t"(result)
		:"0"(x)
	);
	return result;
}
//同时得到sin，cos
void sincos(double x,double *s,double *c)
{
	__asm__ (
		"fsincos;" 
			: "=t" (*c), "=u" (*s) 
			: "0" (x) : "st(7)"
	);
}
//正切
double tangent(double x)
{
	register double result;
	register double __value2 __attribute__ ((unused));
	__asm __volatile__
	(
		"fptan"
		:"=t" (__value2), "=u" (result) 
		:"0" (x)
	);
	return result;
}




double arcworker(double y, double x)
{
	register double result;
	__asm __volatile__
	(
		"fpatan\n\t"
		"fld %%st(0)"
		:"=t" (result)
		:"0" (x), "u" (y)
	);
	return result;
}
//反正弦
double arcsine(double x)
{
	return arcworker (x, squareroot (1.0 - x * x));
}
//反余弦
double arccosine(double x)
{
	return arcworker (squareroot (1.0 - x * x), x);
}
//反正切
double arctangent(double x)
{
	register double result;
	__asm __volatile__
	(
		"fld1\n\t"
		"fpatan"
		: "=t" (result)
		: "0" (x)
	);
	return result;
}




//以2位底的对数
double log2(double x)
{
	register double result;
	__asm __volatile__
	(
		"fld1; fxch; fyl2x" 	//use instrucion,not algorithm
		:"=t" (result) 		//output
		:"0" (x) 		//input
		:"st(1)"		//clobbered register
	);
	return result;
}
//以10位底的对数
double lg(double x)
{
	return log2(x)/log2(10);
}
//以e位底的对数
double ln(double x)
{
	return log2(x) / log2(2.7182818284590452353602874713526624977572470936);
}
//其他对数
double logarithm(double y,double base)	//y=base^x	->	x=log(y,base)
{
	return log2(y)/log2(base);
}




//result=value*(2^exp)
double fscale(double value, int exp)
{
	double temp, texp, temp2;
	texp = exp;

	__asm __volatile__
	(
		"fscale " 
		:"=u" (temp2), "=t" (temp) 
		:"0" (texp), "1" (value)
	);
	return (temp);
}




//result=2^x-1+1
double f2xm1(double x)
{
	double result;
	__asm __volatile__
	(
		"f2xm1"
		:"=t" (result)
		:"0" (x)
	);
	return result+1;
}
//*/result=x^y
double power(double x,double y)
{
	double result;
	short t1,t2;
	__asm __volatile__(
		"fxch\n\t"
		"ftst\n\t"
		"fstsw\n\t"
		"and $0x40,%%ah\n\t"
		"jz 1f\n\t"
		"fstp %%st(0)\n\t"
		"ftst\n\t"
		"fstsw\n\t"
		"fstp %%st(0)\n\t"
		"and $0x40,%%ah\n\t"
		"jnz 0f\n\t"
		"fldz\n\t"
		"jmp 2f\n\t"
		"0:\n\t"
		"fld1\n\t"
		"jmp 2f\n\t"
		"1:\n\t"
		"fstcw %3\n\t"
		"fstcw %4\n\t"
		"orw $0xC00,%4\n\t"
		"fldcw %4\n\t"
		"fld1\n\t"
		"fxch\n\t"
		"fyl2x\n\t"
		"fmulp\n\t"
		"fld %%st(0)\n\t"
		"frndint\n\t"
		"fxch\n\t"
		"fsub %%st(1),%%st(0)\n\t"
		"f2xm1\n\t"
		"fld1\n\t"
		"faddp\n\t"
		"fxch\n\t"
		"fld1\n\t"
		"fscale\n\t"
		"fstp %%st(1)\n\t"
		"fmulp\n\t"
		"fldcw %3\n\t"
		"2:"
			: "=t" (result)
			: "0" (y), "u" (x), "m" (t1), "m" (t2)
			: "st(1)", "st(7)", "%3", "%4", "ax"
	);
	return(result);
}
double exponent(double x)
{
	double result;
	short t1,t2;
	__asm__ (
		"fstcw %2\n\t"
		"fstcw %3\n\t"
		"orw $0xC00,%3\n\t"
		"fldcw %3\n\t"
		"fldl2e\n\t"
		"fmulp\n\t"
		"fld %%st(0)\n\t"
		"frndint\n\t"
		"fxch\n\t"
		"fsub %%st(1),%%st(0)\n\t"
		"f2xm1\n\t"
		"fld1\n\t"
		"faddp\n\t"
		"fxch\n\t"
		"fld1\n\t"
		"fscale\n\t"
		"fstp %%st(1)\n\t"
		"fmulp\n\t"
		"fldcw %2"
			: "=t" (result)
			: "0" (x), "m" (t1), "m" (t2)
			: "st(6)", "st(7)", "%2", "%3"
	);
	return(result);
}




/*
double exponent(double y)
{
	double x=2.7182818284590452353602874713526624977572470936;
    float one = 1;
    double result;
    __asm __volatile__
        (
        "frndint\n"
        ".byte 0xdc, 0xe9\n"
        "fxch\n"
        "fldl %[x]\n"
        "fyl2x\n"
        "f2xm1\n"
        "fadds %[one]\n"
        "fscale\n"
        : "=t"(result)
        : [x]"m"(x), "0"(y), "u"(y), [one]"m"(one)
        : "%st(1)", "%st(7)"
    );
    return result;
}
double power(double base,double exp)
{
	double tmpVal;
	__asm __volatile__
	{
		"fld [exp]\n"
		"fld [base]\n"
		"fyl2x\n"
		"fld1\n"
		"fld st(1)\n"
		"fprem\n"
		"f2xm1\n"
		"faddp st(1),st(0)\n"
		"fscale\n"
		"fxch\n"
		"fstp st(0)\n"
		"fstp [tmpVal]\n"
	}
	return tmpVal;
}
double power(double x, double y)
{
    float one = 1;
    double result;
    __asm __volatile__
	(
        "frndint\n"
        ".byte 0xdc, 0xe9\n"
        "fxch\n"
        "fldl %[x]\n"
        "fyl2x\n"
        "f2xm1\n"
        "fadds %[one]\n"
        "fscale\n"
        : "=t"(result)
        : [x]"m"(x), "0"(y), "u"(y), [one]"m"(one)
        : "%st(1)", "%st(7)"
    );
    return result;
}
double _pow(double a, double b) {
    double c = 1;
    for (int i=0; i<b; i++)
        c *= a;
    return c;
}

double _fact(double x) {
    double ret = 1;
    for (int i=1; i<=x; i++) 
        ret *= i;
    return ret;
}

double _sin(double x) {
    double y = x;
    double s = -1;
    for (int i=3; i<=100; i+=2) {
        y+=s*(_pow(x,i)/_fact(i));
        s *= -1;
    }  
    return y;
}
double _cos(double x) {
    double y = 1;
    double s = -1;
    for (int i=2; i<=100; i+=2) {
        y+=s*(_pow(x,i)/_fact(i));
        s *= -1;
    }  
    return y;
}
double _tan(double x) {
     return (_sin(x)/_cos(x));  
}




double cosine(double x)
{
    double ret=0,item=1.0,temp;
    int n=0,i,sign=1;
    if(x>2*3.1415||x<-2*3.1415){x-=((int)(x/(2*3.1415)))*(2*3.1415);}

    do{
        temp=item;
        for(i=1;i<=2*n;i++)temp/=i;
        ret+=sign*temp;
        item*=x*x;
        sign *=-1;
        n++;
      }while (temp>1e-10);
return ret;
}




double sine(double x)
{
    int m = 1,i;
    double temp,ret = 0.0;
    if(x>2*3.1415||x<-2*3.1415){x-=((int)(x/(2*3.1415)))*(2*3.1415);}

    do{
        i=0;
        if (m%2 == 0){temp= -1.0;}
        else{temp= 1.0;}
        for(i=1;i<=2*m-1;i++){temp = temp * x/i;}
        ret+= temp;
        m++;
    }while (temp<-.0000005||temp>0.0000005);
return ret;
}




double squareroot(double x)
{
    double temp,ret;
    if(x<0){return 0;}

    temp=0.5*(1+x);
    ret=0.5*(temp+x/temp);
    while(1)
    {
	if( (ret-temp) >= 1e-10 ) break;
	if( (temp-ret) >= 1e-10 ) break;
        temp=ret;
        ret=0.5*(temp+x/temp);
    }
return ret;
}
*/
/*
float squareroot(float val)
{
    float result;
    __asm__ __volatile__ ( "fld %1;"
                           "fsqrt;"
                           "fstp %0;" 
                           : "=g" (result) 
                           : "g" (val)
    );
    return result ;
}
*/
/* Convert angle from degrees to radians and then calculate sin value */
/*
float sine(float degree)
{
    float result, two_right_angles = 180.0f ;
    __asm__ __volatile__ ( "fld %1;"
                           "fld %2;"
                           "fldpi;"
                           "fmul;"
                           "fdiv;"
                           "fsin;"
                           "fstp %0;"
                           : "=g" (result) 
                           : "g" (two_right_angles), "g" (degree)
    );
    return result ;
}
*/
/* Convert angle from degrees to radians and then calculate cos value */
/*
float cosine( float degree )
{
    float result, two_right_angles = 180.0f, radians ;
    __asm__ __volatile__ ( "fld %1;"
                           "fld %2;"
                           "fldpi;"
                           "fmul;"
                           "fdiv;"
                           "fstp %0;" 
                           : "=g" (radians)
                           : "g"(two_right_angles), "g" (degree)
    );

    __asm__ __volatile__ ( "fld %1;"
                           "fcos;"
                           "fstp %0;" 
                           : "=g" (result)
                           : "g" (radians)
    );
    return result ;
}
*/
/*
double pow (double x, double y)
{
	register double __value, __exponent;
	long p = (long) y;

	if (x == 0.0 && y > 0.0)return 0.0;
	if (y == (double) p)
	{
		double r = 1.0;
		if (p == 0)return 1.0;
		if (p < 0)
		{
			p = -p;
			x = 1.0 / x;
		}
		while (1)
		{
			if (p & 1)
			r *= x;
			p >>= 1;
			if (p == 0)return r;
			x *= x;
		}
		//NOTREACHED
	}
	__asm __volatile__
    (
		"fmul	%%st(1)		# y * log2(x)\n\t"
		"fstl	%%st(1)\n\t"
		"frndint			# int(y * log2(x))\n\t"
		"fxch\n\t"
		"fsub	%%st(1)		# fract(y * log2(x))\n\t"
		"f2xm1			# 2^(fract(y * log2(x))) - 1\n\t"
		: "=t" (__value), "=u" (__exponent) 
		:  "0" (__log2 (x)), "1" (y)
	);

	__value += 1.0;
	__asm __volatile__
	(
		"fscale"
		: "=t" (__value) 
		: "0" (__value), "u" (__exponent)
	);
	return __value;
}*/
