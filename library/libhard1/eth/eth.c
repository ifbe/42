#include "libhard.h"




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




static u16 swap16(u16 tmp)
{
    return (tmp>>8) | ((tmp&0xff)<<8);
}
static u32 swap32(u32 tmp)
{
    return ((tmp>>24)&0xff) |
           ((tmp>> 8)&0xff00) |
           ((tmp<< 8)&0xff0000) |
           ((tmp<<24)&0xff000000);
}
static u16 checksum(u16 *buf, int len)
{
	u32 sum = 0;
	while(len>1){
		sum += *buf;
		buf += 1;
		len -= 2;
	}
	if(len){
		sum += *(u8*)buf;
	}

	//32位转换成16位
	while(sum>>16){
		sum = (sum>>16) + (sum & 0xffff);
	}
	return (u16)(~sum);
}




int dhcprequest(struct _fullpkt_dhcp4* pkt)
{
	int len;

	//data
	pkt->dhcp.op = 1;
	pkt->dhcp.htype = 1;
	pkt->dhcp.hlen = 6;
	pkt->dhcp.hops = 0;
	pkt->dhcp.xid = 0x89abcdef;
	pkt->dhcp.secs = 0;
	pkt->dhcp.flags = 0;
	pkt->dhcp.ciaddr = 0;
	pkt->dhcp.yiaddr = 0;
	pkt->dhcp.siaddr = 0;
	pkt->dhcp.giaddr = 0;
	pkt->dhcp.chaddr[0] = 0;
	pkt->dhcp.sname[0] = 0;
	pkt->dhcp.file[0] = 0;
	len = 12+16+16+64+128;

	//udp
	len += 8;
	pkt->udp.srcport = swap16(68);
	pkt->udp.dstport = swap16(67);
	pkt->udp.udplen = swap16(len);
	pkt->udp.udpsum = 0;

	//ipv4
	len += 20;
	pkt->ipv4.iphead = 0x45;
	pkt->ipv4.tos = 0;
	pkt->ipv4.length = swap16(len);
	pkt->ipv4.id = 0x233;
	pkt->ipv4.fragoffset = 0;
	pkt->ipv4.ttl = 0x40;
	pkt->ipv4.protocol = 0x11;
	pkt->ipv4.checksum = 0;
	*(u32*)(pkt->ipv4.ipsrc) = 0x00000000;  //0.0.0.0
	*(u32*)(pkt->ipv4.ipdst) = 0xffffffff;  //255.255.255.255
	pkt->ipv4.checksum = checksum((u16*)&pkt->ipv4, 20);

	//ether
	len += 6+6+2;
	*(u64*)(pkt->mac.macdst) = 0xffffffffffff;
	*(u64*)(pkt->mac.macsrc) = 0;       //unknown
	pkt->mac.type = 0x0008;

	return len;
}




int dhcppacket_parse(u8* buf, int len)
{
	//logtoall("dhcp reply:\n");
	//printmemory(buf,len);
	struct _dhcp* p = (void*)buf;
	logtoall("op=%x,htype=%x,hlen=%x,hops=%x\n", p->op, p->htype, p->hlen, p->hops);
	logtoall("xid=%x,sec=%x,flag=%x\n", swap32(p->xid), swap16(p->secs), swap16(p->flags));
	logtoall("ciaddr=%d.%d.%d.%d\n", p->ci[0], p->ci[1], p->ci[2], p->ci[3]);
	logtoall("yiaddr=%d.%d.%d.%d\n", p->yi[0], p->yi[1], p->yi[2], p->yi[3]);
	logtoall("siaddr=%d.%d.%d.%d\n", p->si[0], p->si[1], p->si[2], p->si[3]);
	logtoall("giaddr=%d.%d.%d.%d\n", p->gi[0], p->gi[1], p->gi[2], p->gi[3]);
	logtoall("id@%p\n", p->chaddr);
	logtoall("hostname=%.64s\n", p->sname);
	logtoall("bootfile=%.128s\n", p->file);
	return 0;
}
int udppacket_parse(u8* buf, int len)
{
	struct _udp* p = (void*)buf;
	int src = swap16(p->srcport);
	int dst = swap16(p->dstport);
	//int len = swap16(p->udplen);
	logtoall("srcport=%d,dstport=%d\n", src, dst);
	if((67 == src)&&(68 == dst))dhcppacket_parse(buf+8, len-8);
	//else printmemory(buf+8, len-8);
	return 0;
}




int protocol_parse(u8* buf, int len, int proto)
{
	switch(proto){
	case 0x11:
		udppacket_parse(buf, len);
		break;
/*	case 0x6:
		tcppacket_parse(buf, len);
		break;
*/
	default:
		//printmemory(buf+headlen, len-headlen);
		break;
	}
	return 0;
}
int ipv4packet_parse(u8* buf, int len)
{
	struct _ipv4* p = (void*)buf;
	int headlen = (p->iphead&0xf)<<2;
	int length = swap16(p->length);
	int proto = p->protocol;
	logtoall("length=%x,proto=%x\n", length, proto);
	logtoall("srcaddr=%d.%d.%d.%d, dstaddr=%d.%d.%d.%d\n",
		p->ipsrc[0],p->ipsrc[1],p->ipsrc[2],p->ipsrc[3],
		p->ipdst[0],p->ipdst[1],p->ipdst[2],p->ipdst[3]);

	protocol_parse(buf+headlen, len-headlen, proto);
	return 0;
}
int ipv6packet_parse(u8* buf, int len)
{
	struct _ipv6* p = (void*)buf;
	u32 ipv6head = p->ipv6head;
	u32 length = swap16(p->length);
	u8 proto = p->proto;
	u8 hoplimit = p->hoplimit;
	logtoall("ipv6head=%x,length=%x,proto=%x,hoplimit=%x,ipsrc and ipdst:\n", ipv6head, length, proto, hoplimit);
	printmemory(p->ipsrc, 0x10*2);

	int headlen = 0x28;
	protocol_parse(buf+headlen, len-headlen, proto);
	return 0;
}
/*
int arppacket_parse()
{
	return 0;
}
*/




/*
struct vlanpacket{
	u16 tag;
	u16 pricfivid;
	struct normalpacket;
};
*/
int vlanpacket_parse(u8* buf, int len)
{
	//u16 tag = (buf[0]<<8) + buf[1];
	u16 pricfivid = (buf[2]<<8) + buf[3];
	u16 type = (buf[4]<<8) + buf[5];
	switch(type){
	case 0x0800:ipv4packet_parse(buf+4, len-4);break;
	case 0x86dd:ipv6packet_parse(buf+4, len-4);break;
	}
	return 0;
}
int macpacket_parse(u8* buf, int len)
{
	int headlen = 6+6+2;
	int type = (buf[12]<<8) + buf[13];
	switch(type){
	case 0x0800:
		ipv4packet_parse(buf+headlen, len-headlen);
		break;
	case 0x86dd:
		ipv6packet_parse(buf+headlen, len-headlen);
		break;
/*	case 0x0806:
		arppacket_parse(buf+headlen, len-headlen);
		break;
	case 0x8100:
		vlanpacket_parse(buf+headlen, len-headlen);
		break;*/
	default:
		logtoall("macpacket_parse: unknown packet\n");
		printmemory(buf, len < 16 ? len : 16);
	}
	return 0;
}




int eth_reader(struct item* e1000,void* foot, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
int eth_writer(struct item* e1000,void* foot, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
int eth_delete(struct item* eth,int xxx)
{
	return 0;
}
int eth_create(struct item* eth,int xxx)
{
	return 0;
}




int eth_take(struct item* e1000,void* foot, void* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	return 0;
}
int eth_give(struct item* e1000,void* foot, void* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("@eth_give\n");
	//printmemory(buf,len);

	macpacket_parse(buf, len);
	return 0;
}
int eth_discon(struct item* eth,int xxx, struct item* card,int slot)
{
	return 0;
}
int eth_linkup(struct item* eth,int xxx, struct item* card,int slot)
{
	logtoall("eth_linkup: %p,%x,%p,%x\n", eth,xxx, card,slot);

	eth->take = (void*)eth_take;
	eth->give = (void*)eth_give;

	struct _fullpkt_dhcp4 pkt;
	dhcprequest(&pkt);
	card->give(card,0, 0,0, 0,0, &pkt, sizeof(struct _fullpkt_dhcp4));
	return 0;
}