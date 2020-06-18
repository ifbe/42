#include "struct.h"




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

void gl41line(               struct entity* ctx, u32 rgb, vec3 va, vec3 vb);
void gl41line_shorter(       struct entity* ctx, u32 rgb, vec3 va, vec3 vb);
void gl41line_arrow(         struct entity* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vn);
void gl41line_bezier(        struct entity* ctx, u32 rgb, vec3 va, vec3 vb, vec3 vt);
void gl41line_spring(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_yshape(        struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void gl41line_triangle(      struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void gl41line_rect(          struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41line_rectround(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41line_rectselect(    struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41line_hexagon(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41line_circle(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41line_gear(          struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, int tooth);
void gl41line_rotategear(    struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, int tooth, float a);
void gl41line_cone(          struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41line_prism4(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41line_cylinder(      struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41line_dodecahedron(  struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41line_icosahedron(   struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41line_sphere(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void gl41solid_triangle(     struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void gl41solid_rect(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41solid_circle(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void gl41solid_cone(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vu);
void gl41solid_prism4(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_cask(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_cylinder(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_gear(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int tooth);
void gl41solid_rotategear(   struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int tooth, float a);
void gl41solid_dodecahedron( struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_icosahedron(  struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_sphere(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41solid_propeller(    struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int dir, int dt);

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
void gl41opaque_propeller(   struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu, int dir, int dt);

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
void carvetext(               struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvetext_reverse(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void carvefloat(              struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, float data);
void carvedouble(             struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, double data);

void carveaxis(               struct entity* ctx);
void carvefrustum(            struct entity* ctx, struct fstyle* sty);
void select_3d(               struct entity* ctx, u32 rgb, struct fstyle* sty, u32 flag);