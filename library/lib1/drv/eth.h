#include "const/def.h"




struct _mac{
	//ether
	u8 macdst[6];           //[0,5]
	u8 macsrc[6];           //[6,0xb]
	u16     type;           //[0xc,0xd]
}__attribute__((packed));
struct _ipv4{
	u8      iphead;         //[0]
	u8         tos;         //[1]
	u16     length;         //[2,3]
	u16         id;         //[4,5]
	u16 fragoffset;         //[6,7]
	u8         ttl;         //[8]
	u8    protocol;         //[9]
	u16   checksum;         //[0xa,0xb]
	u8    ipsrc[4];         //[0xc,0xf]
	u8    ipdst[4];         //[0x10,0x13]
}__attribute__((packed));
struct _ipv6{
	u32   ipv6head;         //[0,3]
	u16     length;         //[4,5]
	u8       proto;         //[6]
	u8    hoplimit;         //[7]
	u8   ipsrc[16];         //[0x08,0x17]
	u8   ipdst[16];         //[0x18,0x27]
}__attribute__((packed));
struct _udp{
	u16 srcport;
	u16 dstport;
	u16 udplen;
	u16 udpsum;
}__attribute__((packed));
struct _dhcp{
	u8 op;
	u8 htype;
	u8 hlen;
	u8 hops;
	u32 xid;
	u16 secs;
	u16 flags;
	union{
	u32 ciaddr;
	u8 ci[4];
	};
	union{
	u32 yiaddr;
	u8 yi[4];
	};
	union{
	u32 siaddr;
	u8 si[4];
	};
	union{
	u32 giaddr;
	u8 gi[4];
	};
	u8 chaddr[16];
	u8 sname[64];
	u8 file[128];
}__attribute__((packed));
struct _fullpkt_dhcp4{
	struct _mac mac;

	struct _ipv4 ipv4;

	struct _udp udp;

	struct _dhcp dhcp;
}__attribute__((packed));
