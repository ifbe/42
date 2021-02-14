#include "libhard.h"
//
int doihave8259();
void dual8259_init();
void dual8259_endofirq(int);
void dual8259_enableirq(int);
void dual8259_disableirq(int);
//
void ioapic_init();
void localapic_endofirq(int num);
void ioapic_enableirq(int num);
void ioapic_disableirq(int num);




static int chosen = 8259;
void enableirq(int irq)
{
    if(8259 == chosen){
        dual8259_enableirq(irq);
    }
    else{
       ioapic_enableirq(irq);
    }
}
void disableirq(int irq)
{
    if(8259 == chosen){
        dual8259_disableirq(irq);
    }
    else{
        ioapic_disableirq(irq);
    }
}
void endofextirq(int irq)
{
    if(8259 == chosen){
        dual8259_endofirq(irq);
    }
    else{
        localapic_endofirq(irq);
    }
}
void initirq(struct item* dev)
{
	if(1 == doihave8259()){
		dual8259_init();
        chosen = 8259;
	}
	else{
		ioapic_init();
        chosen = 0;
	}
}