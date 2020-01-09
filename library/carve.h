#include "struct.h"




void carveaxis(               struct entity* ctx);
void carvefrustum(            struct entity* ctx, struct fstyle* sty);
void select_3d(               struct entity* ctx, u32 rgb, struct fstyle* sty, u32 flag);

void gl41point(              struct entity* ctx, u32 rgb, vec3 vc);
void gl41point_bezier(       struct entity* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void gl41point_triangle(     struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void gl41point_rect(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41point_circle(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41point_cone(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41point_cask(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41point_cylinder(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41point_dodecahedron( struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41point_icosahedron(  struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41point_sphere(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carveline(               struct entity* ctx, u32 rgb, vec3 va, vec3 vb);
void carveline_shorter(       struct entity* ctx, u32 rgb, vec3 va, vec3 vb);
void carveline_arrow(         struct entity* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vn);
void carveline_bezier(        struct entity* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void carveline_special(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu, float sa, float da);
void carveline_yshape(        struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline_triangle(      struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carveline_rect(          struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_rectround(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_rectselect(    struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_hexagon(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_circle(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carveline_gear(          struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, int tooth);
void carveline_rotategear(    struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, int tooth, float a);
void carveline_cone(          struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_prism4(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_cylinder(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carveline_dodecahedron(  struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_icosahedron(   struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carveline_sphere(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carvesolid_triangle(     struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void carvesolid_rect(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid_circle(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void carvesolid_cone(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void carvesolid_prism4(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_cask(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_cylinder(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_gear(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int tooth);
void carvesolid_rotategear(   struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int tooth, float a);
void carvesolid_dodecahedron( struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_icosahedron(  struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_sphere(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void carvesolid_propeller(    struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int dir, int dt);

void gl41opaque_triangle(    struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void gl41opaque_rect(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41opaque_circle(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41opaque_cone(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41opaque_prism4(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_cask(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_cylinder(    struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_dodecahedron(struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_icosahedron( struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41opaque_sphere(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void carveascii(              struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void carveascii_center(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void carveunicode(            struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void carveunicode_center(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void carveutf8(               struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);
void carveutf8_center(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);

void carvedecimal(            struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carvehexadecimal(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carvehex8_center(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void carvestring(             struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvestring_center(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvetext_reverse(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvefloat(              struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, float data);
void carvedouble(             struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, double data);
