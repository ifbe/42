#include "libhard.h"
#define _isa_ hex32('i','s','a',0)
#define _gsi_ hex32('g','s','i',0)
//
int acpi_getdual8259();
void* acpi_getirqioapic();
void* acpi_getredirtbl();
//
void dual8259_init();
void dual8259_endofirq(int);
void dual8259_enableirq(int);
void dual8259_disableirq(int);
//
void ioapic_init();
void localapic_endofirq(int num);
void ioapic_enableirq(int pinnum, int intval);
void ioapic_disableirq(int num);




static int chosen = 0;




//example:
//chip=isa, pin=0, cpu=0, int=0x20
//chip=ioapic, pin=2, cpu=0, int=0x20
void irqchip_enableirq(int chip, int pin, int apicid, int intvec)
{
	if(0 == chip)chip = _isa_;	//default pass isa value

	if(8259 == chosen){
		dual8259_enableirq(pin);
	}
	else{
		u8 gsi = pin;
		if(_isa_ == chip){
			u8* redirtbl = acpi_getredirtbl();
			gsi = redirtbl[pin];
		}
		say("@enableirq: isa %d -> gsi %d\n", pin, gsi);
		ioapic_enableirq(gsi, intvec);
	}
}
void irqchip_disableirq(int chip, int pin)
{
	if(8259 == chosen){
		dual8259_disableirq(pin);
	}
	else{
		u8* redirtbl = acpi_getredirtbl();
		u8 gsi = redirtbl[pin];
		say("@disableirq: isa %d -> gsi %d\n", pin, gsi);
		ioapic_disableirq(gsi);
	}
}
void irqchip_endofirq(int irq)
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