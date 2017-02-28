#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char




//--------------------------------world---------------------------------
#define input_char 0x72616863	//char
#define input_kbd 0x64626b	//kbd
	#define kbd_esc 0x1b
	#define kbd_left 0x25
	#define kbd_up 0x26
	#define kbd_right 0x27
	#define kbd_down 0x28
	#define kbd_f1 0x70
	#define kbd_f2 0x71
	#define kbd_f3 0x72
	#define kbd_f4 0x73
	#define kbd_f5 0x74
	#define kbd_f6 0x75
	#define kbd_f7 0x76
	#define kbd_f8 0x77

#define point_down 0x2b70	//p+
#define point_up 0x2d70		//p-
#define point_move 0x4070	//p@
	#define touch_0 0
	#define touch_1 1
	#define touch_2 2
	#define touch_3 3
	#define touch_4 4
	#define touch_5 5
	#define touch_6 6
	#define touch_7 7
	#define touch_8 8
	#define touch_9 9
	#define btn_left 'l'
	#define btn_right 'r'
	#define btn_middle 'm'
	#define btn_front 'f'
	#define btn_back 'b'
	#define btn_side1 '1'
	#define btn_side2 '2'
	#define btn_side3 '3'
	#define btn_side4 '4'
	#define btn_side5 '5'

#define net_in 0x2b6e		//n+
#define net_out 0x2d6e		//n-
#define net_data 0x406e		//n@

#define drag_file 0x656c6966	//file
#define drag_data 0x61746164	//data

#define screen_size 0x657a6973	//size

struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};

u64* eventread();
void eventwrite(u64 what, u64 who, u64 where, u64 when);
void birth();
void death();
//----------------------------------------------------------------------




//--------------------------libui---------------------------------------
#define uibase 0x000000		//xlib, winapi, gtk, gl...
	#define uibase0 uibase+0x80000
	#define uibase1 uibase+0x00000
#define uiuser 0x100000		//2048, spectrum, camera...
	#define gamehome uiuser+0x00000
	#define toolhome uiuser+0x40000
#define uinode 0x200000		//item
#define uidata 0x300000		//data

struct player
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	u8 data[0xc0];
};
struct window
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;
	u64 startx;
	u64 starty;
	u64 lenx;
	u64 leny;

	u8 data[0xc0];
};

//libui1
void charactercreate();
void characterdelete();
void characterstart(char* addr, char* fmt, int w, int h);
void characterstop();
void characterlist();
void characterchange();
void characterread();
void characterwrite();
void charactercommand(void*);

//libui0
void windowcreate();
void windowdelete();
void windowstart(char* addr, char* fmt, int w, int h);
void windowstop();
void windowlist();
void windowchange();
void windowread();
void windowwrite();
//----------------------------------------------------------------------




//-------------------------------libsoft--------------------------------
#define fsbase 0x000000		//fd
	#define fsbase0 fsbase+0x80000
	#define fsbase1 fsbase+0x00000
#define fsuser 0x100000		//worker
	#define memhome fsuser+0
	#define nethome fsuser+0xd0000
	#define syshome fsuser+0xe0000
	#define wirehome fsuser+0xf0000
#define fsnode 0x200000		//dir
#define fsdata 0x300000		//raw

struct worker
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	u8 data[0xc0];
};
struct object
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;
	u64 a;
	u64 b;
	u64 c;
	u64 d;

	u8 data[0xc0];
};

//libsoft1
void background1(struct window* win);
void backgroundcolor(struct window* win,
	u32 color);

void line(struct window* win,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void radial(struct window* win,
	int x1, int y1,
	int x2, int y2,
	u32 color);

void circlebody(struct window* win,
	int x, int y,
	int r, u32 body);
void circleframe(struct window* win,
	int x, int y,
	int r, u32 frame);
void circle(struct window* win,
	int x, int y,
	int r, u32 body, u32 frame);

void rectbody(struct window* win,
	int x1, int y1,
	int x2, int y2,
	u32 body);
void rectframe(struct window* win,
	int x1, int y1,
	int x2, int y2,
	u32 frame);
void rect(struct window* win,
	int x1, int y1,
	int x2, int y2,
	u32 body, u32 frame);

void trianglebody(struct window* win,
	int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	u32 body, u32 frame);
void triangleframe(struct window* win,
	int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	u32 body, u32 frame);
void triangle(struct window* win,
	int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	u32 body, u32 frame);

void printascii(struct window* win,
	int x, int y, int size,
	char ch, u32 fg, u32 bg);
void printbyte(struct window* win,
	int x, int y, int size,
	char ch, u32 fg, u32 bg);
void printstring(struct window* win,
	int x, int y, int size,
	char* s, u32 fg, u32 bg);
void printdecimal(struct window* win,
	int x, int y, int size,
	u64 dat, u32 fg, u32 bg);
void printhexadecimal(struct window* win,
	int x, int y, int size,
	u64 dat, u32 fg, u32 bg);
void printfloat(struct window* win,
	int x, int y, int size,
	float dat, u32 fg, u32 bg);
void printdouble(struct window* win,
	int x, int y, int size,
	double dat, u32 fg, u32 bg);

void defaultascii(struct window* win,
	int x, int y, u8 ch);
void defaultstring(struct window* win,
	int x, int y, u8* s);
void defaulthexadecimal(struct window* win,
	int x, int y, u64 hex);
void defaultdecimal(struct window* win,
	int x, int y, long long dec);
void defaultdouble(struct window* win,
	int x, int y, double data);

int bigdup(
	u8* src, int sl,
	u8* dst, int dl);
int bigcmp(
	u8* src, int sl,
	u8* dst, int dl);
int bigshl(
	u8* buf, int len,
	int sh);
int bigshr(
	u8* buf, int len,
	int sh);
int bigadd(
	u8* abuf, int alen,
	u8* bbuf, int blen);
int bigadd_muled(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	int val);
int bigsub(
	u8* abuf, int alen,
	u8* bbuf, int blen);
int bigsub_muled(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	int val);
int bigmul(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	u8* answer, int max);
int bigpow(
	u8* base, int bl,
	u8* exp, int el,
	u8* mod, int ml,
	u8* ans, int al,
	u8* t1, int l1);
int bigdiv(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	u8* quotient, int max1,
	u8* remainder, int max2);
int rsa2048(
	u8* dstbuf, int dstlen,
	u8* srcbuf, int srclen,
	u8* keybuf, int keylen,
	u8* modbuf, int modlen);

int data2decstr(u64 data,u8* str);
int data2hexstr(u64 data,u8* str);
int hexstr2data(u8* src,u64* dst);
int decstr2data(u8* src,u64* dst);

int bigint2decstr(u8* src,u8* dst,int count);
int bigint2hexstr(u8* src,u8* dst,int count);
int hexstr2bigint(u8* src, u8* dst);
int decstr2bigint(u8* src, u8* dst);

int decstr2double(u8* src,double* dst);
int hexstr2double(u8* src,double* dst);
int double2decstr(double src,u8* dst);
int double2hexstr(double src,u8* dst);

int utf2unicode(u8* src,u32* dst);
int unicode2utf(u32 src,u8* dst);

int cmp(u8*, u8*);
int ncmp(u8*, u8*, int);
int wildcard(u8* wild,u8* str);

int copy(u8* dst, u8* src);		//strcpy
int ncopy(u8* dst, u8* src, int len);	//strncpy
int movsb(u8* dst, u8* src, int len);

int findhead(u8* buf);
int findtail(u8* buf);
int findzero(u8* buf);
int finddata(u8* buf, u8 data);

int md5sum(u8* dst, u8* src, int len);
int sha1sum(u8* dst, u8* src, int len);
int sha256sum(u8* dst, u8* src, int len);
int sha512sum(u8* dst, u8* src, int len);

int buf2typename(u8* p,int max,u64* type,u8** name);
int buf2optval(u8* pp,int max,u8** type,u8** name);
int buf2suffix(u8* p,u8** suffix);
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2addrport(u8* pp,int max,u8* addr,int* port);

u8* buf2folder(u8* p);
u8* buf2filename(u8* p);

//libsoft0
int readfile(u8* file, u8* mem, u64 offset, u64 count);
int writefile(u8* file, u8* mem, u64 offset, u64 count);
u32 getrandom();
u64 gettime();
//----------------------------------------------------------------------




//-------------------------------libhard--------------------------------
#define hwbase 0x000000		//cpu, ether, sound, video...
	#define hwbase0 hwbase+0x80000
	#define hwbase1 hwbase+0x00000
#define hwuser 0x100000		//driver
	#define diskhome hwuser+0x00000
	#define usbhome hwuser+0x40000
	#define tcpiphome hwuser+0x80000
	#define yyhome hwuser+0xc0000
#define hwnode 0x200000		//struct
#define hwdata 0x300000		//content

struct driver
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	u8 data[0xc0];
};
struct hardware
{
	u64 buf;
	u64 fmt;
	u64 off;
	u64 len;
	u64 a;
	u64 b;
	u64 c;
	u64 d;

	u8 data[0xc0];
};

//libhard1
int initusb();
int initnet();

//libhard0
int initahci(u8* hba);
int initxhci(u8* hba);
//----------------------------------------------------------------------




//-------------------------------libboot--------------------------------
#define xxbase 0x000000		//acpi, dts, jtag, int
	#define xxbase0 xxbase+0x80000
	#define xxbase1 xxbase+0x00000
#define xxuser 0x100000		//configer
#define xxnode 0x200000		//type
#define xxdata 0x300000		//value

struct explainer
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	u8 data[0xc0];
};
struct devicetree
{
	u64 buf;
	u64 fmt;
	u64 off;
	u64 len;
	u64 a;
	u64 b;
	u64 c;
	u64 d;

	u8 data[0xc0];
};

//libboot1
void printmemory(char*,int);
void say(char*,...);

//libboot0
void power_off();
void power_on();
//----------------------------------------------------------------------
