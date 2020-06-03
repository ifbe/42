#include "libsoft.h"




float inertia_scalar_of_stick(float M, float L)
{
	return M * L * L / 12;
}
void  inertia_tensor_of_stick(mat3 Ival, mat3 Iinv, float M, float L)
{
}




float inertia_scalar_of_cube(float M, float R)
{
	return M * R * R / 6;
}
void  inertia_tensor_of_cube(mat3 Ival, mat3 Iinv, float M, float R)
{
}




float inertia_scalar_of_block(float M, float lx, float ly, float lz)
{
	return (lx*lx + ly*ly) * M / 12;
}
void  inertia_tensor_of_block(mat3 Ival, mat3 Iinv, float M, float lx, float ly, float lz)
{
	float Ixx = (ly*ly + lz*lz) * M / 12;
	float Iyy = (lx*lx + lz*lz) * M / 12;
	float Izz = (lx*lx + ly*ly) * M / 12;
	if(Ival){
		Ival[0][0] = Ixx;Ival[0][1] = 0.0;Ival[0][2] = 0.0;
		Ival[1][0] = 0.0;Ival[1][1] = Iyy;Ival[1][2] = 0.0;
		Ival[2][0] = 0.0;Ival[2][1] = 0.0;Ival[2][2] = Izz;
	}
	if(Iinv){
		Iinv[0][0] = 1.0 / Ixx;Iinv[0][1] = 0.0;Iinv[0][2] = 0.0;
		Iinv[1][0] = 0.0;Iinv[1][1] = 1.0 / Iyy;Iinv[1][2] = 0.0;
		Iinv[2][0] = 0.0;Iinv[2][1] = 0.0;Iinv[2][2] = 1.0 / Izz;
	}
}




float inertia_scalar_of_sphere(float M, float R)
{
	return M * R * R * 0.4;
}
void  inertia_tensor_of_sphere(mat3 Ival, mat3 Iinv, float M, float R)
{
}
