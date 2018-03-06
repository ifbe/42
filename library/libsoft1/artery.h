#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define __fd__ hex32('f','d',0,0)
#define __uart__ hex32('u','a','r','t')




struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
struct object
{
	//[0x00,0x0f]
	u64 sock:56;	//raw, bt, udp, tcp?
	u64 stage0:8;
	u64 type:56;	//http, tls, ssh
	u64 stage1:8;
	union{
		void* irel;
		u64 pad0;
	};
	union{
		void* orel;
		u64 pad1;
	};

	//[0x20,0x3f]
	u64 fd;
	u64 flag;
	u64 len;
	union{
		u64 addr;
		void* buf;
	};

	//[0x40,0x7f]
	u8 self[0x20];
	u8 peer[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};
struct element
{
	//[00,20]
	u64 type;
	u64 name;
	union{
		void* irel;
		u64 pad0;
	};
	union{
		void* orel;
		u64 pad1;
	};

	//[20,3f]
	u64 info;
	u64 flag;
	u64 len;
	u64 buf;

	//[40,ff]
	u8 data[0xc0];
};




//
u32 getrandom();
u64 gettime();
void eventwrite(u64,u64,u64,u64);
void* eventread();
//
int mysnprintf(void*, int, void*, ...);
void printmemory(void*, int);
void say(void*, ...);
