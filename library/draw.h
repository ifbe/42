#include "struct.h"




void drawline(          struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_arrow(    struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_bezier(   struct entity* ctx, u32 rgb, int ax, int ay, int bx, int by, int cx, int cy);
void drawline_triangle( struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawline_rect(     struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_hexagon(  struct entity* ctx, u32 rgb, int cx, int cy, int rx, int ry);
void drawline_circle(   struct entity* ctx, u32 rgb, int cx, int cy, int r);
void drawline_oval(     struct entity* ctx, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawline_sector(   struct entity* ctx, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawsolid_triangle(struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawsolid_rect(    struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawsolid_circle(  struct entity* ctx, u32 rgb, int cx, int cy, int r);
void drawsolid_oval(    struct entity* ctx, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawsolid_sector(  struct entity* ctx, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawicon_1(        struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1);
void drawopaque_rect(   struct entity* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawopaque_circle( struct entity* ctx, u32 rgb, int cx, int cy, int r);

void drawascii(         struct entity* ctx, u32 rgb, int cx, int cy, u8 data);
void drawbyte(          struct entity* ctx, u32 rgb, int cx, int cy, u8 data);
void drawhex32(         struct entity* ctx, u32 rgb, int cx, int cy, u32 data);
void drawunicode(       struct entity* ctx, u32 rgb, int cx, int cy, u32 unicode);
void drawutf8(          struct entity* ctx, u32 rgb, int cx, int cy, u8* buf, int len);
void drawstring(        struct entity* ctx, u32 rgb, int cx, int cy, u8* buf, int len);
void drawdecimal(       struct entity* ctx, u32 rgb, int cx, int cy, int data);
void drawhexadecimal(   struct entity* ctx, u32 rgb, int cx, int cy, u64 data);
void drawdouble(        struct entity* ctx, u32 rgb, int cx, int cy, double z);

void drawascii_fit(     struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8 data);
void drawunicode_fit(   struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u32 unicode);
void drawutf8_fit(      struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawstring_fit(    struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawdec_fit(       struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, int);
void drawhex_fit(       struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u64);

int drawtext(           struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
int drawtext_reverse(   struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
int drawvt100(          struct entity* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
