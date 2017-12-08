#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define __fd__ hex32('f','d',0,0)




struct object
{
	//[0x00,0x0f]
	u64 type_sock;	//raw, bt, udp, tcp?
	u64 haha;
	union{
		void* first;
		u64 pad0;
	};
	union{
		void* last;
		u64 pad1;
	};

	//[0x20,0x3f]
	u64 type_road;	//http2, ws, rdp, vnc?
	u64 stage1;
	u64 type_data;	//html, rgb?
	u64 stage3;

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
	u64 id;
	union{
		void* first;
		u64 pad0;
	};
	union{
		void* last;
		u64 pad1;
	};

	//[20,3f]
	u64 buf;
	u64 len;
	u64 info;
	u64 flag;

	//[40,77]
	union{
		int (*create)();
		char padding0[8];
	};
	union{
		int (*delete)();
		char padding1[8];
	};
	union{
		int (*start)();
		char padding2[8];
	};
	union{
		int (*stop)();
		char padding3[8];
	};
	union{
		int (*list)();
		char padding4[8];
	};
	union{
		int (*choose)();
		char padding5[8];
	};
	union{
		int (*read)(void* win, void* act, void* style, void* player);
		char padding6[8];
	};
	union{
		int (*write)(void* event);
		char padding7[8];
	};

	//[80,ff]
	u8 data[0x80];
};




//
u32 getrandom();
u64 gettime();
//
int mysnprintf(void*, int, void*, ...);
void printmemory(void*, int);
void say(void*, ...);