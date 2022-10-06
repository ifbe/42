#include "libhard.h"
//
int acpi_getdual8259();
void* acpi_getirqioapic();
//
void dual8259_init();
void dual8259_endofirq(int);
void dual8259_enableirq(int);
void dual8259_disableirq(int);
//
void ioapic_init();
void localapic_endofirq(int num);
void ioapic_enableirq(int num);
void ioapic_disableirq(int num);




static int chosen = 0;
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
	say("@initirq\n");
	if(0 != acpi_getirqioapic()){
		ioapic_init();
		chosen = 0;
		say("irqchip=apic\n");
	}
	else if(1 == acpi_getdual8259()){
		dual8259_init();
		chosen = 8259;
		say("irqchip=8259\n");
	}
	else{
		say("error: no irq chip found\n");
		while(1)asm("hlt");
	}
	say("@initirq end\n\n");
}