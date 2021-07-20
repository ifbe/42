#include "bignode.h"




void drawline(          _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_arrow(    _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_bezier(   _obj* ctx, u32 rgb, int ax, int ay, int bx, int by, int cx, int cy);
void drawline_triangle( _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawline_rect(     _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_choose(   _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawline_hexagon(  _obj* ctx, u32 rgb, int cx, int cy, int rx, int ry);
void drawline_circle(   _obj* ctx, u32 rgb, int cx, int cy, int r);
void drawline_oval(     _obj* ctx, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawline_sector(   _obj* ctx, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawsolid_triangle(_obj* ctx, u32 rgb, int x1, int y1, int x2, int y2, int x3, int y3);
void drawsolid_rect(    _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawsolid_circle(  _obj* ctx, u32 rgb, int cx, int cy, int r);
void drawsolid_oval(    _obj* ctx, u32 rgb, int cx, int cy, int rx, int ry, int fx, int fy);
void drawsolid_sector(  _obj* ctx, u32 rgb, int cx, int cy, int radius, int start, int end);

void drawicon_1(        _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1);
void drawopaque_rect(   _obj* ctx, u32 rgb, int x1, int y1, int x2, int y2);
void drawopaque_circle( _obj* ctx, u32 rgb, int cx, int cy, int r);

void drawascii(         _obj* ctx, u32 rgb, int cx, int cy, u8 data);
void drawstring(        _obj* ctx, u32 rgb, int cx, int cy, u8* buf, int len);
void drawfloat(         _obj* ctx, u32 rgb, int cx, int cy, float z);
void drawdouble(        _obj* ctx, u32 rgb, int cx, int cy, double z);
void drawhex8(          _obj* ctx, u32 rgb, int cx, int cy, u8 data);
void drawhex32(         _obj* ctx, u32 rgb, int cx, int cy, u32 data);
void drawhexadecimal(   _obj* ctx, u32 rgb, int cx, int cy, u64 data);
void drawdec8(          _obj* ctx, u32 rgb, int cx, int cy, u8 data);
void drawdecimal(       _obj* ctx, u32 rgb, int cx, int cy, int data);
void drawunicode(       _obj* ctx, u32 rgb, int cx, int cy, u32 unicode);
void drawutf8(          _obj* ctx, u32 rgb, int cx, int cy, u8* buf, int len);

void drawascii_fit(     _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8 data);
void drawunicode_fit(   _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u32 unicode);
void drawutf8_fit(      _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawstring_fit(    _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
void drawdec_fit(       _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, int);
void drawhex_fit(       _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u64);

int drawtext(           _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
int drawtext_reverse(   _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
int drawvt100(          _obj* ctx, u32 rgb, int x0, int y0, int x1, int y1, u8* buf, int len);
