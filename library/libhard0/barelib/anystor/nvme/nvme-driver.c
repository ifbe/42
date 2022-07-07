#include "libhard.h"
#define nvme_print(fmt, ...) say("<%08lld,nvme>" fmt, timeread_us(), ##__VA_ARGS__)




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