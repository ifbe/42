

#define ARM_TIMER 0xb000
//
#define LOAD    0x400	//Load
#define VALUE   0x404	//Value (Read-Only)
#define CONTROL 0x408	//Control
#define IRQCNTL 0x40c	//IRQ Clear/Ack (Write-Only)
#define RAWIRQ  0x410	//RAW IRQ (Read-Only)
#define MSKIRQ  0x414	//Masked IRQ (Read-Only)
#define RELOAD  0x418	//Reload
#define PREDIV  0x41c	//Pre-divider (Not in real 804!)
#define FREECNT 0x420	//Free running counter (Not in real 804!)
//timer_clock = apb_clock / (pre_devider+1)