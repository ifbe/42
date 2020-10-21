#include "libhard.h"
//asid
//npd
#define VMEXIT_CR_READ            0    //read of CR 0 through 15, respectively
#define VMEXIT_CR_WRITE        0x10    //write of CR 0 through 15, respectively
#define VMEXIT_DR_READ         0x20    //read of DR 0 through 15, respectively
#define VMEXIT_DR_WRITE        0x30    //write of DR 0 through 15, respectively
#define VMEXIT_EXCP            0x40    //exception vector 0–31, respectively
#define VMEXIT_INTR            0x60	//physical INTR (maskable interrupt)
#define VMEXIT_NMI             0x61	//physical NMI
#define VMEXIT_SMI             0x62	//physical SMI (the EXITINFO1 field provides more information)
#define VMEXIT_INIT            0x63	//physical INIT
#define VMEXIT_VINTR           0x64	//virtual INTR
#define VMEXIT_CR0_SEL_WRITE   0x65	//write of CR0 that changed any bits other than CR0.TS or CR0.MP
#define VMEXIT_IDTR_READ       0x66	//read of IDTR
#define VMEXIT_GDTR_READ       0x67	//read of GDTR
#define VMEXIT_LDTR_READ       0x68	//read of LDTR
#define VMEXIT_TR_READ         0x69	//read of TR
#define VMEXIT_IDTR_WRITE      0x6A	//write of IDTR
#define VMEXIT_GDTR_WRITE      0x6B	//write of GDTR
#define VMEXIT_LDTR_WRITE      0x6C	//write of LDTR
#define VMEXIT_TR_WRITE        0x6D	//write of TR
#define VMEXIT_RDTSC           0x6E	//RDTSC instruction
#define VMEXIT_RDPMC           0x6F	//RDPMC instruction
#define VMEXIT_PUSHF           0x70	//PUSHF instruction
#define VMEXIT_POPF            0x71	//POPF instruction
#define VMEXIT_CPUID           0x72	//CPUID instruction
#define VMEXIT_RSM             0x73	//RSM instruction
#define VMEXIT_IRET            0x74	//IRET instruction
#define VMEXIT_SWINT           0x75	//software interrupt (INTn instructions)
#define VMEXIT_INVD            0x76	//INVD instruction
#define VMEXIT_PAUSE           0x77	//PAUSE instruction
#define VMEXIT_HLT             0x78	//HLT instruction
#define VMEXIT_INVLPG          0x79	//INVLPG instructions
#define VMEXIT_INVLPGA         0x7A	//INVLPGA instruction
#define VMEXIT_IOIO            0x7B	//IN or OUT accessing protected port (the EXITINFO1 field provides more information)
#define VMEXIT_MSR             0x7C	//RDMSR or WRMSR access to protected MSR
#define VMEXIT_TASK_SWITCH     0x7D	//task switch
#define VMEXIT_FERR_FREEZE     0x7E	//FP legacy handling enabled, and processor is frozen in an x87/mmx instruction waiting for an interrupt
#define VMEXIT_SHUTDOWN        0x7F	//Shutdown
#define VMEXIT_VMRUN           0x80	//VMRUN instruction
#define VMEXIT_VMMCALL         0x81	//VMMCALL instruction
#define VMEXIT_VMLOAD          0x82	//VMLOAD instruction
#define VMEXIT_VMSAVE          0x83	//VMSAVE instruction
#define VMEXIT_STGI            0x84	//STGI instruction
#define VMEXIT_CLGI            0x85	//CLGI instruction
#define VMEXIT_SKINIT          0x86	//SKINIT instruction
#define VMEXIT_RDTSCP          0x87	//RDTSCP instruction
#define VMEXIT_ICEBP           0x88	//ICEBP instruction
#define VMEXIT_WBINVD          0x89	//WBINVD or WBNOINVD instruction
#define VMEXIT_MONITOR         0x8A	//MONITOR or MONITORX instruction
#define VMEXIT_MWAIT           0x8B	//MWAIT or MWAITX instruction
#define VMEXIT_MWAIT_CONDITION 0x8C	//MWAIT or MWAITX instruction, if monitor hardware is armed.
#define VMEXIT_XSETBV          0x8D	//XSETBV instruction
#define VMEXIT_RDPRU           0x8E	//RDPRU instruction
#define VMEXIT_EFER_WRITE_TRAP 0x8F	//Write of EFER MSR (occurs after guest instruction finishes)
#define VMEXIT_CR_WRITE_TRAP   0x90	//Write of CR0-15, respectively (occurs after guest instruction finishes)
#define VMEXIT_MCOMMIT         0xA3	//MCOMMIT instruction
#define VMEXIT_NPF            0x400	//Nested paging: host-level page fault occurred (EXITINFO1 contains fault error code; EXITINFO2 contains the guest physical address causing the fault.)
#define AVIC_INCOMPLETE_IPI   0x401	//AVIC—Virtual IPI delivery not completed. See "AVIC IPI Delivery Not Completed" for EXITINFO1–2 definitions.
#define AVIC_NOACCEL          0x402	//AVIC—Attemped access by guest to vAPIC register not handled by AVIC hardware. See "AVIC Access to un-accelerated vAPIC register" for EXITINFO1–2 definitions.
#define VMEXIT_VMGEXIT        0x403	//VMGEXIT instruction
#define VMEXIT_INVALID           –1	//Invalid guest state in VMCB
#define VMEXIT_BUSY              –2	//BUSY bit was set in the encrypted VMSA (see "Interrupt Injection Restrictions")
struct VMCB_control{
	//[0,3]
	u16 cr_read;
	u16 cr_write;
	//[4,7]
	u16 dr_read;
	u16 dr_write;
	//[8,b]
	u32 exception;
	//[c,f]
	u32       intr:1; //.0
	u32        nmi:1;
	u32        smi:1;
	u32       init:1;
	u32      vintr:1; //.4
	u32       cr0w:1;
	u32  idtr_read:1;
	u32  gdtr_read:1;
	u32  ldtr_read:1; //.8
	u32    tr_read:1;
	u32 idtr_write:1;
	u32 gdtr_write:1;
	u32 ldtr_write:1; //.12
	u32   tr_write:1;
	u32      rdtsc:1;
	u32      rdpmc:1;
	u32      pushf:1; //.16
	u32       popf:1;
	u32      cpuid:1;
	u32        rsm:1;
	u32       iret:1; //.20
	u32       intn:1;
	u32       invd:1;
	u32      pause:1;
	u32        hlt:1; //.24
	u32     invlpg:1;
	u32    invlpga:1;
	u32         io:1;
	u32        msr:1; //.28
	u32         ts:1;
	u32     freeze:1;
	u32   shutdown:1;
	//[10,11]
	u16 vmrun:1; //0
	u16 vmmcall:1;
	u16 vmload:1;
	u16 vmsave:1;
	u16 stgi:1; //4
	u16 clgi:1;
	u16 skinit:1;
	u16 rdtscp:1;
	u16 icebp:1; //8
	u16 wbinvd:1;
	u16 monitor:1;
	u16 mwait_uncon:1;
	u16 mwait_armed:1; //c
	u16 xsetbv:1;
	u16 rdpru:1;
	u16 efer_write_after:1;
	//[12,13]
	u16 cr_write_after;
	//[14,17]
	u32 rsvd_14_0: 3;
	u32   mcommit: 1;
	u32 rsvd_14_4:28;
	//[18,3b]
	u8 rsvd_18[36];
	//[3c,3f]
	u16 pause_thres;
	u16 pause_count;
	//[40,47]
	u64 addr_iopm;
	//[48,4f]
	u64 addr_msrpm;
	//[50,57]
	u64 tsc_offset;
	//[58,5f]
	u32 GuestAsid;
	u8 TlbControl;
	u8 rsvd_5d;
	//[60,67]
	u8 v_tpr;       //60
	u8 v_irq:1;     //61
	u8 vgif:1;
	u8 rsvd_61:6;
	u8 v_intr_prio:4;   //62
	u8 v_ign_tpr:1;
	u8 rsvd_62:3;
	u8 v_intr_masking:1;    //63
	u8 AMDVirtualGIF:1;
	u8 rsvd_63:5;
	u8 AVICEnable:1;
	u8 v_intr_vector;   //64
	u8 rsvd_65[3];
	//[68,6f]
	u64 interrupt_shadow:1;
	u64 guest_interrupt_mask:1;
	u64 rsvd_68:62;
	//[70,77]
	u64 exitcode;
	//[78,7f]
	u64 exitinfo1;
	//[80,87]
	u64 exitinfo2;
	//[88,8f]
	u64 exitinfoint;
	//[90,97]
	u64 np_enable:1;
	u64 EnableSecureEncrypted:1;
	u64 EnableEncryptedState:1;
	u64 GuestModeExecuteTrap:1;
	u64 rsvd_90_4:1;
	u64 VirtualTransparentEncryption:1;
	u64 rsvd_90_6:58;
	//[98,9f]
	u64 AVICAPICBAR:52;
	u64 rsvd_98:10;
	//[a0,a7]
	u64 GuestGHCB;
	//[a8,af]
	u64 EventInjection;
	//[b0,b7]
	u64 n_cr3;
	//[b8,bf]
	u64 lbr_virt_enable:1;
	u64 virt_vmsave_vmload:1;
	u64 rsvd_b8:62;
	//[c0,c7]
	u32 VMCBCleanBits;
	u32 rsvd_c4;
	//[c8,cf]
	u64 nRIP;
	//[d0,df]
	u8 NumberOfByteFetched;
	u8 Guestinstruction[15];
	//[e0,e7]
	u64 AVIC_APIC_BACK_PAGE:52;
	u64 rsvd_e0:10;
	//[e8,ef]
	u64 rsvd_e8;
	//[f0,f7]
	u64 rsvd_f0_0:12;
	u64 AVIC_LOGICAL_TABLE_Pointer:40;
	u64 rsvd_f0_52:12;
	//[f8,ff]
	u64 AVIC_PHYSICAL_MAX_INDEX:8;
	u64 rsvd_f8_8:4;
	u64 AVIC_PHYSICAL_TABLE_Pointer_shift12:40;
	u64 rsvd_f8_52:12;
	//[100,107]
	u64 rsvd_100;
	//[108,10f]
	u64 rsvd_108_0:12;
	u64 VMSA_Pointer_shift12:40;
	u64 rsvd_108_52:12;
	//All other fields up to 3FFh rsvd
}__attribute__((packed));
struct VMCB_statedata{
	//[00,0f]
	u16 ES_selector;
	u16 ES_attrib;
	u32 ES_limit;
	u64 ES_base;
	//[10,1f]
	u16 CS_selector;
	u16 CS_attrib;
	u32 CS_limit;
	u64 CS_base;
	//[20,2f]
	u16 SS_selector;
	u16 SS_attrib;
	u32 SS_limit;
	u64 SS_base;
	//[30,3f]
	u16 DS_selector;
	u16 DS_attrib;
	u32 DS_limit;
	u64 DS_base;
	//[40,4f]
	u16 FS_selector;
	u16 FS_attrib;
	u32 FS_limit;
	u64 FS_base;
	//[50,5f]
	u16 GS_selector;
	u16 GS_attrib;
	u32 GS_limit;
	u64 GS_base;
	//[60,6f]
	u16 GDTR_selector;
	u16 GDTR_attrib;
	u32 GDTR_limit;
	u64 GDTR_base;
	//[70,7f]
	u16 LDTR_selector;
	u16 LDTR_attrib;
	u32 LDTR_limit;
	u64 LDTR_base;
	//[80,8f]
	u16 IDTR_selector;
	u16 IDTR_attrib;
	u32 IDTR_limit;
	u64 IDTR_base;
	//[90,9f]
	u16 TR_selector;
	u16 TR_attrib;
	u32 TR_limit;
	u64 TR_base;
	//[a0,ca]
	u8 rsvd_a0[0x2b];
	//cb
	u8 CPL;
	//[cc,cf]
	u32 rsvd_cc;
	//[d0,d7]
	u64 efer;
	//[d8,147]
	u8 rsvd_d8[0x70];
	//[148,14f]
	u64 cr4;
	//[150,157]
	u64 cr3;
	//[158,15f]
	u64 cr0;
	//[160,167]
	u64 cr7;
	//[168,16f]
	u64 cr6;
	//[170,177]
	u64 rflags;
	//[178,17f]
	u64 rip;
	//[180,1d7]
	u8 rsvd_180[0x58];
	//[1d8,1df]
	u64 rsp;
	//[1e0,1f7]
	u8 rsvd_1e0[0x20];
	//[1f8,1ff]
	u64 rax;
	//[200,207]
	u64 star;
	//[208,20f]
	u64 lstar;
	//[210,217]
	u64 cstar;
	//[218,21f]
	u64 sfmask;
	//[220,227]
	u64 KernelGsBase;
	//[228,22f]
	u64 SYSENTER_CS;
	//[230,237]
	u64 SYSENTER_ESP;
	//[238,23f]
	u64 SYSENTER_EIP;
	//[240,247]
	u64 cr2;
	//[248,267]
	u8 rsvd_248[0x20];
	//[268,26f]
	u64 G_PAT;
	//[270,277]
	u64 DBGCTL;
	//[278,27f]
	u64 BR_FROM;
	//[280,287]
	u64 BR_TO;
	//[288,28f]
	u64 LAST_EXCP_FROM;
	//[290,297]
	u64 LAST_EXCP_TO;
	//298h to end rsvd
}__attribute__((packed));
struct vmcb{
	union{
		u8 size1k[0x400];
		struct VMCB_control ctl;
	};
	union{
		u8 size2k[0x800];
		struct VMCB_statedata dat;
	};
}__attribute__((packed));




//register rax, physical address, mapped as writeback
void vmrun(void* vmcb)
{
}