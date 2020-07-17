#include "struct.h"


void dx11point(              struct entity* ctx, u32 rgb, vec3 vc);

void dx11line(               struct entity* ctx, u32 rgb, vec3 va, vec3 vb);
void dx11line_rect(          struct entity* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf);

void dx11solid_triangle(     struct entity* ctx, u32 rgb, vec3 v0, vec3 v1, vec3 v2);
void dx11solid_rect(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf);
void dx11solid_prism4(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void dx11solid_cylinder(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, vec3 vu);

void dx11ascii(              struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void dx11ascii_center(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void dx11unicode(            struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void dx11unicode_center(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void dx11utf8(               struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);
void dx11utf8_center(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);

void dx11decimal(             struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);




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

void gl41ascii(              struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void gl41ascii_center(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8 dat);
void gl41unicode(            struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void gl41unicode_center(     struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 uni);
void gl41utf8(               struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);
void gl41utf8_center(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* buf, int len);

void gl41decimal(             struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void gl41hexadecimal(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void gl41hex8_center(         struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u32 dat);
void gl41string(              struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void gl41string_center(       struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void gl41text(                struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void gl41text_reverse(        struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, u8* str, int len);
void gl41float(               struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, float data);
void gl41double(              struct entity* ctx, u32 rgb, vec3 vc, vec3 vr, vec3 vf, double data);

void gl41axis(                struct entity* ctx);
void gl41frustum(             struct entity* ctx, struct fstyle* sty);
void gl41boundingvolume(      struct entity* ctx, u32 rgb, struct fstyle* sty, u32 flag);
