#include "libsoft.h"
void mat3_transposefrom(void* o, void* i);
void mat3_multiplyfrom(void* o, void* l, void* r);




float inertia_scalar_of_stick(float M, float L)
{
	return M * L * L / 12;
}
float inertia_scalar_of_cube(float M, float R)
{
	return M * R * R / 6;
}
float inertia_scalar_of_block(float M, float lx, float ly, float lz)
{
	return M * (lx*lx + ly*ly) / 12;
}
float inertia_scalar_of_sphere(float M, float R)
{
	return M * R * R * 2 / 5;
}
float inertia_scalar_of_ellipsoid(float M, float a, float b)
{
	return M * (a*a + b*b) / 5;
}
float inertia_scalar_parallel_axis(float M, float distance, float inertia)
{
	return M * distance*distance + inertia;
}




/*
 ∫(rz²+ry²)dm	-∫( rx*ry )dm	-∫( rx*rz )dm
-∫( ry*rx )dm	 ∫(rz²+rx²)dm	-∫( ry*rz )dm
-∫( rz*rx )dm	-∫( rz*ry )dm	 ∫(rx²+ry²)dm
*/
void inertia_tensor_of_cube(mat3 Ival, float M, float L)
{
	float Ixx = M * (L * L / 6);
	float Iyy = Ixx;
	float Izz = Ixx;
	if(Ival){
		Ival[0][0] = Ixx;	Ival[0][1] = 0.0;	Ival[0][2] = 0.0;
		Ival[1][0] = 0.0;	Ival[1][1] = Iyy;	Ival[1][2] = 0.0;
		Ival[2][0] = 0.0;	Ival[2][1] = 0.0;	Ival[2][2] = Izz;
	}
}
void inertia_tensor_of_block(mat3 Ival, float M, float lx, float ly, float lz)
{
	float Ixx = M * (ly*ly + lz*lz) / 12;
	float Iyy = M * (lx*lx + lz*lz) / 12;
	float Izz = M * (lx*lx + ly*ly) / 12;
	if(Ival){
		Ival[0][0] = Ixx;	Ival[0][1] = 0.0;	Ival[0][2] = 0.0;
		Ival[1][0] = 0.0;	Ival[1][1] = Iyy;	Ival[1][2] = 0.0;
		Ival[2][0] = 0.0;	Ival[2][1] = 0.0;	Ival[2][2] = Izz;
	}
}
void inertia_tensor_of_cylinder(mat3 Ival, float M, float R, float H)
{
	float Ixx = M * (R * R / 4 + H * H / 12);
	float Iyy = Ixx;
	float Izz = M * (R * R / 2);
	if(Ival){
		Ival[0][0] = Ixx;	Ival[0][1] = 0.0;	Ival[0][2] = 0.0;
		Ival[1][0] = 0.0;	Ival[1][1] = Iyy;	Ival[1][2] = 0.0;
		Ival[2][0] = 0.0;	Ival[2][1] = 0.0;	Ival[2][2] = Izz;
	}
}
void inertia_tensor_of_sphere(mat3 Ival, float M, float R)
{
	float Ixx = M * R * R * 2 / 5;
	float Iyy = Ixx;
	float Izz = Ixx;
	if(Ival){
		Ival[0][0] = Ixx;	Ival[0][1] = 0.0;	Ival[0][2] = 0.0;
		Ival[1][0] = 0.0;	Ival[1][1] = Iyy;	Ival[1][2] = 0.0;
		Ival[2][0] = 0.0;	Ival[2][1] = 0.0;	Ival[2][2] = Izz;
	}
}
void inertia_tensor_of_ellipsoid(mat3 Ival, float M, float a, float b, float c)
{
	float Ixx = M * (b*b + c*c) / 5;
	float Iyy = M * (a*a + c*c) / 5;
	float Izz = M * (a*a + b*b) / 5;
	if(Ival){
		Ival[0][0] = Ixx;	Ival[0][1] = 0.0;	Ival[0][2] = 0.0;
		Ival[1][0] = 0.0;	Ival[1][1] = Iyy;	Ival[1][2] = 0.0;
		Ival[2][0] = 0.0;	Ival[2][1] = 0.0;	Ival[2][2] = Izz;
	}
}





void inertia_tensor_local2world(mat3 Iworld, mat3 Ilocal, mat3 mat)
{
	mat3 Tmat;
	mat3_transposefrom(Tmat, mat);

	//Iworld = mat^T * Ilocal * mat
	mat3 tmp;
	mat3_multiplyfrom(tmp, Ilocal, mat);
	mat3_multiplyfrom(Iworld, Tmat, tmp);
}