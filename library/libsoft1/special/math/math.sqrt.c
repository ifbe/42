#define eps 0.0001




float sqrt_dichotomy(float n)
{
	float mid,last; 
	float low,up; 
	if(n<0)return n; 

	low=0;
	up=n; 
	mid=(low+up)/2; 
	while(1)
	{
		if(mid*mid>n)up=mid; 
		else low=mid;

		last=mid;
		mid=(up+low)/2; 

		if(mid-last > eps)continue;
		else if(mid-last < -eps)continue;
		else break;
	}

	return mid; 
}




float sqrt_newton(float x)
{
	float val = x; //最终
	float last; //保存上一个计算的值

	while(1)
	{
		last = val;
		val = (val + x/val) / 2;

		if(val-last > eps)continue;
		else if(val-last < -eps)continue;
		else break;
	}

	return val;
}




float sqrt_quake(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;

	i = 0x5f375a86 - (i>>1);
	x = *(float*)&i;
	x = x*(1.5f-xhalf*x*x);
	x = x*(1.5f-xhalf*x*x);
	x = x*(1.5f-xhalf*x*x);

	return 1/x;
}
