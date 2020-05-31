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
    Ival[0][0] = (ly*ly + lz*lz) * M / 12;
    Ival[0][1] = 0.0;
    Ival[0][2] = 0.0;

    Ival[1][0] = 0.0;
    Ival[1][1] = (lx*lx + lz*lz) * M / 12;
    Ival[1][2] = 0.0;

    Ival[2][0] = 0.0;
    Ival[2][1] = 0.0;
    Ival[2][2] = (lx*lx + ly*ly) * M / 12;
}




float inertia_scalar_of_sphere(float M, float R)
{
    return M * R * R * 0.4;
}
void  inertia_tensor_of_sphere(mat3 Ival, mat3 Iinv, float M, float R)
{
}
