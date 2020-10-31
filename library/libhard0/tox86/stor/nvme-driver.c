#include "libhard.h"
#define nvme_print(fmt, ...) say("<%08lld,nvme>" fmt, timeread(), ##__VA_ARGS__)
u32 in32(u16 port);
void out32(u16 port, u32 data);




struct NVME_MMIO{
	volatile u32    CAP_lo;	//[ 0, 3]: Controller Capabilities lo32
	volatile u32    CAP_hi;	//[ 4, 7]: Controller Capabilities hi32
	volatile u32        VS;	//[ 8, b]: Version
	volatile u32     INTMS;	//[ c, f]: Interrupt Mask Set
	volatile u32     INTMC;	//[10,13]: Interrupt Mask Clear
	volatile u32        CC;	//[14,17]: Controller Configuration
	volatile u32   rsvd_18;	//[18,1b]
	volatile u32      CSTS;	//[1c,1f]: Controller Status
	volatile u32      NSSR;	//[20,23]: NVM Subsystem Reset (Optional)
	volatile u32       AQA;	//[24,27]: Admin Queue Attributes
	volatile u32    ASQ_lo;	//[28,2b]: Admin Submission Queue Base Address lo32
	volatile u32    ASQ_hi;	//[2c,2f]: Admin Submission Queue Base Address hi32
	volatile u32    ACQ_lo;	//[30,33]: Admin Completion Queue Base Address lo32
	volatile u32    ACQ_hi;	//[34,37]: Admin Completion Queue Base Address hi32
	volatile u32    CMBLOC;	//[38,3b]: Controller Memory Buffer Location (Optional)
	volatile u32    CMBSZ ;	//[3c,3f]: Controller Memory Buffer Size (Optional)
	volatile u32  BPINFO  ;	//[40,43]: Boot Partition Information (Optional)
	volatile u32  BPRSEL  ;	//[44,47]: Boot Partition Read Select (Optional)
	volatile u32  BPMBL_lo;	//[48,4b]: Boot Partition Memory Buffer Location lo32 (Optional)
	volatile u32  BPMBL_hi;	//[4c,4f]: Boot Partition Memory Buffer Location hi32 (Optional)
	volatile u32 CMBMSC_lo;	//[50,53]: Controller Memory Buffer Memory Space Control lo32 (Optional)
	volatile u32 CMBMSC_hi;	//[54,57]: Controller Memory Buffer Memory Space Control hi32 (Optional)
	volatile u32 CMBSTS   ;	//[58,5b]: Controller Memory Buffer Status (Optional)
	u8      rsvd_5c[0xda4];	//[5c,dff]
	volatile u32 PMRCAP   ;	//[e00,e03]: Persistent Memory Capabilities (Optional)
	volatile u32 PMRCTL   ;	//[e04,e07]: Persistent Memory Region Control (Optional)
	volatile u32 PMRSTS   ;	//[e08,e0b]: Persistent Memory Region Status (Optional)
	volatile u32 PMREBS   ;	//[e0c,e0f]: Persistent Memory Region Elasticity Buffer Size
	volatile u32 PMRSWTP  ;	//[e10,e13]: Persistent Memory Region Sustained Write Throughput
	volatile u32 PMRMSC_lo;	//[e14,e17]: Persistent Memory Region Controller Memory Space Control lo32 (Optional)
	volatile u32 PMRMSC_hi;	//[e18,e1b]: Persistent Memory Region Controller Memory Space Control hi32 (Optional)
	u8     rsvd_e1c[0x1e4];	//[e1c,fff]

	//[1000,?]: enq.tail0, deq.head0, enq.tail1, deq.head1...
}__attribute__((packed));




int nvme_mmioinit(struct item* dev, u8* mmio)
{
	nvme_print("mmio@%p{\n", mmio);
	printmmio(mmio, 32);
	nvme_print("}\n");
	return 0;
}




int nvme_portcaps(struct item* dev, u32 addr)
{
	u32 temp,next;

	out32(0xcf8, addr+0x34);
	next = in32(0xcfc)&0xff;
	nvme_print("pcicap@%x\n", next);

	while(1){
		if(next < 0x40)break;
		if(next > 0xfc)break;

		out32(0xcf8, addr+next);
		temp = in32(0xcfc);
		next = (temp>>8)&0xff;
		nvme_print("cap:type=%x,next=%x\n", temp&0xff, next);
	}
	return 0;
}
int nvme_portinit(struct item* dev, u32 addr)
{
	u32 tmp;
	u64 lo,hi;
	nvme_print("port@%x{\n",addr);

	out32(0xcf8, addr+8);
	tmp = in32(0xcfc);
	switch(tmp>>8){
	case 0x010802:nvme_print("nvme ioctl\n");break;
	case 0x010803:nvme_print("nvme admin\n");break;
	}

	out32(0xcf8, addr+0x10);
	lo = in32(0xcfc);
	nvme_print("bar0=%x\n", lo);
	out32(0xcf8, addr+0x14);
	hi = in32(0xcfc);
	nvme_print("bar1=%x\n", hi);
	out32(0xcf8, addr+0x18);
	nvme_print("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	nvme_print("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	nvme_print("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	nvme_print("bar5=%x\n", in32(0xcfc));

	nvme_portcaps(dev, addr);
	nvme_print("}\n");

	//nvme mmio
	u8* mmio = (u8*)((lo&0xffffc000) | (hi<<32));
	return nvme_mmioinit(dev, mmio);
}