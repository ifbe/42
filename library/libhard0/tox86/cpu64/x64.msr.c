//Performance
#define MSR_TSC          0x0010	//“Time-Stamp Counter” on page 373
//System Software
#define MSR_APIC_BASE    0x001B	//“Local APIC Enable” on page 565
//Performance
#define MSR_MPERF        0x00E7	//“Determining Processor Effective Frequency” on page 594
#define MSR_APERF        0x00E8	//“Determining Processor Effective Frequency” on page 594
//Memory Typing
#define MSR_MTRRcap      0x00FE	//“Identifying MTRR Features” on page 197
//System Software
#define MSR_SYSENTER_CS  0x0174	//“SYSENTER and SYSEXIT MSRs” on page 156
#define MSR_SYSENTER_ESP 0x0175
#define MSR_SYSENTER_EIP 0x0176
//Machine Check
#define MSR_MCG_CAP      0x0179	//“Machine-Check Global-Capabilities Register” on page 270
#define MSR_MCG_STATUS   0x017A	//“Machine-Check Global-Status Register” on page 271
#define MSR_MCG_CTL      0x017B	//“Machine-Check Global-Control Register” on page 272
//Software Debug
#define MSR_DebugCtl         0x01D9	//“Debug-Control MSR (DebugCtl)” on page 357
#define MSR_LastBranchFromIP 0x01DB	//“Control-Transfer Recording MSRs” on page 359
#define MSR_LastBranchToIP   0x01DC
#define MSR_LastIntFromIP    0x01DD
#define MSR_LastIntToIP      0x01DE
//Memory Typing
#define MSR_MTRRphysBase0 0x0200	//“Variable-Range MTRRs” on page 194
#define MSR_MTRRphysMask0 0x0201
#define MSR_MTRRphysBase1 0x0202
#define MSR_MTRRphysMask1 0x0203
#define MSR_MTRRphysBase2 0x0204
#define MSR_MTRRphysMask2 0x0205
#define MSR_MTRRphysBase3 0x0206
#define MSR_MTRRphysMask3 0x0207
#define MSR_MTRRphysBase4 0x0208
#define MSR_MTRRphysMask4 0x0209
#define MSR_MTRRphysBase5 0x020A
#define MSR_MTRRphysMask5 0x020B
#define MSR_MTRRphysBase6 0x020C
#define MSR_MTRRphysMask6 0x020D
#define MSR_MTRRphysBase7 0x020E
#define MSR_MTRRphysMask7 0x020F
//Memory Typing
#define MSR_MTRRfix64K_00000 0x0250	//“Fixed-Range MTRRs” on page 192
#define MSR_MTRRfix16K_80000 0x0258
#define MSR_MTRRfix16K_A0000 0x0259
#define MSR_MTRRfix4K_C0000  0x0268
#define MSR_MTRRfix4K_C8000  0x0269
#define MSR_MTRRfix4K_D0000  0x026A
#define MSR_MTRRfix4K_D8000  0x026B
#define MSR_MTRRfix4K_E0000  0x026C
#define MSR_MTRRfix4K_E8000  0x026D
#define MSR_MTRRfix4K_F0000  0x026E
#define MSR_MTRRfix4K_F8000  0x026F
//Memory Typing
#define MSR_PAT         0x0277	//“PAT Register” on page 200
#define MSR_MTRRdefType 0x02FF	//“Default-Range MTRRs” on page 196
//Machine Check
#define MSR_MC0_CTL    0x0400	//See the documentation for particular implementations of the architecture.
#define MSR_MC0_STATUS 0x0401	//“Machine-Check Status Registers” on page 275
#define MSR_MC0_ADDR   0x0402	//“Machine-Check Address Registers” on page 278
#define MSR_MC0_MISC   0x0403	//“Machine-Check Miscellaneous-Error Information Register 0(MCi_MISC0)” on page 278
#define MSR_MC1_CTL    0x0404
#define MSR_MC1_STATUS 0x0405
#define MSR_MC1_ADDR   0x0406
#define MSR_MC1_MISC   0x0407
#define MSR_MC2_CTL    0x0408
#define MSR_MC2_STATUS 0x0409
#define MSR_MC2_ADDR   0x040A
#define MSR_MC2_MISC   0x040B
#define MSR_MC3_CTL    0x040C
#define MSR_MC3_STATUS 0x040D
#define MSR_MC3_ADDR   0x040E
#define MSR_MC3_MISC   0x040F
#define MSR_MC4_CTL    0x0410
#define MSR_MC4_STATUS 0x0411
#define MSR_MC4_ADDR   0x0412
#define MSR_MC4_MISC   0x0413
#define MSR_MC5_CTL    0x0414
#define MSR_MC5_STATUS 0x0415
#define MSR_MC5_ADDR   0x0416
#define MSR_MC5_MISC   0x0417
//System Software
#define MSR_EFER         0xC0000080	//“Extended Feature Enable Register (EFER)” on page 55
#define MSR_STAR         0xC0000081	//“SYSCALL and SYSRET MSRs” on page 155
#define MSR_LSTAR        0xC0000082
#define MSR_CSTAR        0xC0000083
#define MSR_SF_MASK      0xC0000084
#define MSR_FSBase       0xC0000100	//“FS and GS Registers in 64-Bit Mode” on page 74
#define MSR_GSBase       0xC0000101
#define MSR_KernelGSbase 0xC0000102	//“SWAPGS Instruction” on page 157
#define MSR_TSC_AUX      0xC0000103	//System Software	//“RDTSCP Instruction” on page 159
#define MSR_MC4_MISC1    0xC0000408	//Machine Check	//“Machine-Check Miscellaneous-Error Information Register 0(MCi_MISC0)” on page 278
#define MSR_MC4_MISC2    0xC0000409
#define MSR_MC4_MISC3    0xC000040A
//Software Debug
#define MSR_DR1_ADDR_MASK 0xC0001019	//“Debug Breakpoint Address Masking” on page 366
#define MSR_DR2_ADDR_MASK 0xC000101A
#define MSR_DR3_ADDR_MASK 0xC000101B
#define MSR_DR0_ADDR_MASK 0xC0001027
//Performance
#define MSR_PerfEvtSel0 0xC0010000	//“Core Performance Event-Select Registers” on page 368
#define MSR_PerfEvtSel1 0xC0010001
#define MSR_PerfEvtSel2 0xC0010002
#define MSR_PerfEvtSel3 0xC0010003
#define MSR_PerfCtr0    0xC0010004	//“Performance Counter MSRs” on page 367
#define MSR_PerfCtr1    0xC0010005
#define MSR_PerfCtr2    0xC0010006
#define MSR_PerfCtr3    0xC0010007
//Memory Typing
#define MSR_SYSCFG    0xC0010010	//“System Configuration Register (SYSCFG)” on page 59
#define MSR_IORRBase0 0xC0010016	//“IORRs” on page 206
#define MSR_IORRMask0 0xC0010017
#define MSR_IORRBase1 0xC0010018
#define MSR_IORRMask1 0xC0010019
#define MSR_TOP_MEM   0xC001001A	//“Top of Memory” on page 208
#define MSR_TOP_MEM2  0xC001001D
//CPUID Name
#define MSR_Processor_Name_String0 0xC0010030	//See appropriate BIOS and Kernel Developer’s Guide (BKDG) or Processor Programming Reference Manual for details.
#define MSR_Processor_Name_String1 0xC0010031
#define MSR_Processor_Name_String2 0xC0010032
#define MSR_Processor_Name_String3 0xC0010033
#define MSR_Processor_Name_String4 0xC0010034
#define MSR_Processor_Name_String5 0xC0010035
//SMM
#define MSR_SMI_Trigger_IO_Cycle 0xC0010056	//See appropriate BIOS and Kernel Developer’s Guide (BKDG) or Processor Programming Reference Manual for details.
#define MSR_PStateCurrentLimit   0xC0010061	//“Hardware Performance Monitoring and Control” on page 591
#define MSR_PStateControl        0xC0010062
#define MSR_PStateStatus         0xC0010063
//Machine Check
#define MSR_CPU_Watchdog_Timer 0xC0010074	//“CPU Watchdog Timer Register” on page 272
#define MSR_TSCRatio           0xC0010104	//“TSC Ratio MSR (C000_0104h)” on page 532
//SVM
#define MSR_SMBASE   0xC0010111	//“SMBASE Register” on page 289
#define MSR_SMM_ADDR 0xC0010112	//“SMRAM Protected Areas” on page 295
#define MSR_SMM_MASK 0xC0010113
//SVM
#define MSR_VM_CR       0xC0010114	//“SVM Related MSRs” on page 530
#define MSR_IGNNE       0xC0010115	//“SVM Related MSRs” on page 530
#define MSR_SMM_CTL     0xC0010116	//“SVM Related MSRs” on page 530
#define MSR_VM_HSAVE_PA 0xC0010117	//“SVM Related MSRs” on page 530
#define MSR_SVM_KEY_MSR 0xC0010118	//“SVM-Lock” on page 533
//SMM
#define MSR_SMM_KEY_MSR      0xC0010119	//“SMM-Lock” on page 534
#define MSR_Local_SMI_Status 0xC001011A	//See appropriate BIOS and Kernel Developer’s Guide (BKDG) or Processor Programming Reference Manual for details.
//SVM
#define MSR_DoorbellRegister 0xC001011B	//“Doorbell Register” on page 527
#define MSR_VMPAGE_FLUSH     0xC001011E	//“Secure Encrypted Virtualization” on page 535
#define MSR_GHCB             0xC0010130	//“Guest-HV Communication Block" (see “GHCB” on page 547)
#define MSR_SEV_STATUS       0xC0010131	//"SEV_STATUS MSR" (see “SEV_STATUS MSR” on page 541)
#define MSR_RMP_BASE         0xC0010132	//"Initializing the RMP" (see “Initializing the RMP” on page 551)
#define MSR_RMP_END          0xC0010133	//"Initializing the RMP" (see “Initializing the RMP” on page 551)
//OSVW
#define MSR_OSVW_ID_Length 0xC0010140	//“OS-Visible Workarounds” on page 637
#define MSR_OSVW Status    0xC0010141