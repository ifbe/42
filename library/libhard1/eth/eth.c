#include "libhard.h"




struct udp{
	u16 srcport;
	u16 dstport;
	u16 udplen;
	u16 udpsum;
};
struct ipv4{
	u8      iphead;         //[0xe]
	u8         tos;         //[0xf]
	u16     length;         //[0x10,0x11]
	u16         id;         //[0x12,0x13]
	u16 fragoffset;         //[0x14,0x15]
	u8         ttl;         //[0x16]
	u8    protocol;         //[0x17]
	u16   checksum;         //[0x18,0x19]
	u8    ipsrc[4];         //[0x1a,0x1d]
	u8    ipdst[4];         //[0x1e,0x21]
};
struct dhcp{
	//ether
	u8 macdst[6];           //[0,5]
	u8 macsrc[6];           //[6,0xb]
	u16     type;           //[0xc,0xd]

	//ipv4
	u8      iphead;         //[0xe]
	u8         tos;         //[0xf]
	u16     length;         //[0x10,0x11]
	u16         id;         //[0x12,0x13]
	u16 fragoffset;         //[0x14,0x15]
	u8         ttl;         //[0x16]
	u8    protocol;         //[0x17]
	u16   checksum;         //[0x18,0x19]
	u8    ipsrc[4];         //[0x1a,0x1d]
	u8    ipdst[4];         //[0x1e,0x21]

	//udp
	u16 srcport;
	u16 dstport;
	u16 udplen;
	u16 udpsum;

	//dhcp
	u8 op;
	u8 htype;
	u8 hlen;
	u8 hops;
	u32 xid;
	u16 secs;
	u16 flags;
	u32 ciaddr;
	u32 yiaddr;
	u32 siaddr;
	u32 giaddr;
	u8 chaddr[16];
	u8 sname[64];
	u8 file[128];
};




static u16 swap16(u16 tmp)
{
    return (tmp>>8) | ((tmp&0xff)<<8);
}
u16 checksum(u16 *buf, int len)
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



void dhcprequest(struct dhcp* dhcp)
{
	int len;

	//data
	dhcp->op = 1;
	dhcp->htype = 1;
	dhcp->hlen = 6;
	dhcp->hops = 0;
	dhcp->xid = 0x89abcdef;
	dhcp->secs = 0;
	dhcp->flags = 0;
	dhcp->ciaddr = 0;
	dhcp->yiaddr = 0;
	dhcp->siaddr = 0;
	dhcp->giaddr = 0;
	dhcp->chaddr[0] = 0;
	dhcp->sname[0] = 0;
	dhcp->file[0] = 0;
	len = 12+16+16+64+128;

	//udp
	len += 8;
	dhcp->srcport = swap16(68);
	dhcp->dstport = swap16(67);
	dhcp->udplen = swap16(len);
	dhcp->udpsum = 0;

	//ipv4
	len += 20;
	dhcp->iphead = 0x45;
	dhcp->tos = 0;
	dhcp->length = swap16(len);
	dhcp->id = 0x233;
	dhcp->fragoffset = 0;
	dhcp->ttl = 0x40;
	dhcp->protocol = 0x11;
	dhcp->checksum = 0;
	*(u32*)(dhcp->ipsrc) = 0x00000000;  //0.0.0.0
	*(u32*)(dhcp->ipdst) = 0xffffffff;  //255.255.255.255
	dhcp->checksum = checksum((u16*)&dhcp->iphead, 20);

	//ether
	*(u64*)(dhcp->macdst) = 0xffffffffffff;
	*(u64*)(dhcp->macsrc) = 0;       //unknown
	dhcp->type = 0x0008;
}




int parsepacket_udp(u8* buf, int len)
{
	struct udp* p = (void*)buf;
	int src = swap16(p->srcport);
	int dst = swap16(p->dstport);
	//int len = swap16(p->udplen);
	say("srcport=%d,dstport=%d\n", src, dst);
	printmemory(buf+8, len-8);
}
int parsepacket_ipv4(u8* buf, int len)
{
	struct ipv4* p = (void*)buf;
	int headlen = (p->iphead&0xf)<<2;
	int length = swap16(p->length);
	int proto = p->protocol;
	say("length=%x,proto=%x\n", length, proto);
	say("srcaddr=%d.%d.%d.%d, dstaddr=%d.%d.%d.%d\n",
		p->ipsrc[0],p->ipsrc[1],p->ipsrc[2],p->ipsrc[3],
		p->ipdst[0],p->ipdst[1],p->ipdst[2],p->ipdst[3]);
	switch(proto){
	case 0x11:parsepacket_udp(buf+headlen, len-headlen);break;
	default:printmemory(buf+headlen, len-headlen);
	}
}
int parsepacket_mac(u8* buf, int len)
{
	int ret = 6+6+2;
	printmemory(buf, ret);

	int type = (buf[12]<<8) + buf[13];
	switch(type){
	case 0x0800:parsepacket_ipv4(buf+ret, len-ret);break;
	default:printmemory(buf+ret, len-ret);
	}
	return ret;
}




int eth_take(struct item* e1000,void* foot, void* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	return 0;
}
int eth_give(struct item* e1000,void* foot, void* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	say("@eth_give\n");
	//printmemory(buf,len);

	parsepacket_mac(buf, len);
	return 0;
}




int eth_discon(struct item* eth,int xxx, struct item* card,int slot)
{
	return 0;
}
int eth_linkup(struct item* eth,int xxx, struct item* card,int slot)
{
	say("eth_linkup: %p,%x,%p,%x\n", eth,xxx, card,slot);

	eth->take = (void*)eth_take;
	eth->give = (void*)eth_give;

	struct dhcp dhcp;
	dhcprequest(&dhcp);
	card->give(card,0, 0,0, 0,0, &dhcp, sizeof(struct dhcp));
	return 0;
}