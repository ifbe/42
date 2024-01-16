#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void logtoall(char*,...);




//绝对值
double getabs(double x)
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
//取整
double getfloor(double x)
{
	register double value;
	__volatile unsigned short int cw, cwtmp;

	__asm __volatile("fnstcw %0":"=m"(cw));
	cwtmp = (cw & 0xf3ff) | 0x0400;

	__asm __volatile("fldcw %0"::"m"(cwtmp));
	__asm __volatile("frndint":"=t"(value):"0"(x));
	__asm __volatile("fldcw %0"::"m"(cw));
	return value;
}
double getceil(double x)
{
	register double __value;
	__volatile unsigned short int __cw, __cwtmp;

	__asm __volatile("fnstcw %0" : "=m" (__cw));
	__cwtmp = (__cw & 0xf3ff) | 0x0800; /* rounding up */

	__asm __volatile("fldcw %0" : : "m" (__cwtmp));
	__asm __volatile("frndint" : "=t" (__value) : "0" (x));
	__asm __volatile("fldcw %0" : : "m" (__cw));

	return __value;
}
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




double gettan(double x)
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
double gettanyx(double y, double x)
{
	return gettan(y/x);
}
double getsin(double x)
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
double getcos(double x)
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
void sincos(double x,double *s,double *c)
{
	__asm__ (
		"fsincos;" 
		: "=t" (*c), "=u" (*s) 
		: "0" (x)
		//: "st(7)"
	);
}




double arctanyx(double y, double x)
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
double arctan(double a)
{
	return arctanyx(a, 1.0);
}
double arcsin(double x)
{
	return arctanyx(x, squareroot(1.0 - x * x));
}
double arccos(double x)
{
	return arctanyx(squareroot(1.0 - x * x), x);
}




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
double lg(double x)
{
	return log2(x)/log2(10);
}
double ln(double x)
{
	return log2(x) / log2(2.7182818284590452353602874713526624977572470936);
}
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
//result=2^x-1
double f2xm1(double x)
{
	double result;
	__asm __volatile__
	(
		"f2xm1"
		:"=t" (result)
		:"0" (x)
	);
	return result;
}
//result=x^y
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
			: "%3", "%4", "ax"//"st(1)", "st(7)", 
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
			: "%2", "%3"//"st(6)", "st(7)", 
	);
	return(result);
}




void fpu_fxsave(u64 addr)
{
	asm("movq %0, %%rax\n"
		"fxsave (%%rax)\n"
		:
		:"m"(addr)
	);
}
void fpu_fxrstor(u64 addr)
{
	asm("movq %0, %%rax\n"
		"fxrstor (%%rax)\n"
		:
		:"m"(addr)
	);
}
void fpu_xsave(u64 addr)
{
	asm("movq %0, %%rcx;\n"
		"movl $0xffffffff, %%edx;\n"
		"movl $0xffffffff, %%eax;\n"
		"xsaveopt (%%rcx);\n"
		:
		:"m"(addr)
	);
}
void fpu_rstor(u64 addr)
{
	asm("movq %0, %%rcx;\n"
		"movl $0xffffffff, %%edx;\n"
		"movl $0xffffffff, %%eax;\n"
		"xrstor (%%rcx);\n"
		:
		:"m"(addr)
	);
}