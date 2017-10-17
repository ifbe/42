#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




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
	//[00,07]
	u64 type;

	//[08,0f]
	u64 id;

	//[10,17]
	int (*start)(u8*);
	char padding2[ 8 - sizeof(char*) ];

	//[18,1f]
	int (*stop)();
	char padding3[ 8 - sizeof(char*) ];

	//[20,27]
	int (*list)(u8*);
	char padding4[ 8 - sizeof(char*) ];

	//[28,2f]
	int (*choose)(u8*);
	char padding5[ 8 - sizeof(char*) ];

	//[30,37]
	int (*read)(u8*);
	char padding6[ 8 - sizeof(char*) ];

	//[38,3f]
	int (*write)(u8*);
	char padding7[ 8 - sizeof(char*) ];

	//[40,ff]
	u8 data[0xc0];
};




//
u32 getrandom();
u64 gettime();
//
int mysnprintf(void*, int, void*, ...);
void printmemory(void*, int);
void say(void*, ...);