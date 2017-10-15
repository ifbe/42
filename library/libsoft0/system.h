#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct object
{
	//[0x00,0x0f]
	u64 type_sock;	//raw, bt, udp, tcp?
	u64 stage0;
	u64 type_road;	//ssh, tls?
	u64 stage1;

	//[0x20,0x3f]
	u64 type_app;	//http2, ws, rdp, vnc?
	u64 stage2;
	u64 type_data;	//html, rgb?
	u64 stage3;

	//[0x40,0x7f]
	u8 self[0x20];
	u8 peer[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};