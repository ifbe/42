#include "libuser.h"




/*
u32 rtx(vec3 ro, vec3 rd)
{
	//find nearest
	for(){
		ret = intersect(ro,rd, t0,t1,t2);
		if(0 == ret)return;

		if(out.z < depth){
			depth = out.z;
			min = this;
		}
	}

	//this color + that color ?
	u32 c0 = frag(nearest point);
	u32 cn = 0;
	for(){
		ko ,kd = newrayfromthis(ro, rd);
		c += rtx(ko, kd);
	}
	return c0*t + cn*(1-t);
}
void raytrace()
{
	ro, rd;
	for(y=0;y<height;y++){
		for(x=0;x<width;x++){
			ro, rd = mat_2dto3d();
			color = rtx(ro, rd);
		}
	}
}
*/
