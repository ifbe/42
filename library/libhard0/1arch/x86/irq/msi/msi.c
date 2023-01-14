#include "libhard.h"




/*
Fields in the Message Address Register are as follows:
Bits 31-20 — These bits contain a fixed value for interrupt messages (0FEEH).
	This value locates interrupts at the 1-MByte area with a base address of 4G – 18M. All accesses to this region are directed as interrupt messages. Care must to be taken to ensure that no other device claims the region as I/O space.
Destination ID — This field contains an 8-bit destination ID.
	It identifies the message’s target processor(s). The destination ID corresponds to bits 63:56 of the I/O APIC Redirection Table Entry if the IOAPIC is used to dispatch the interrupt to the processor(s).
Redirection hint indication (RH) — When this bit is set, the message is directed to the processor with the lowest interrupt priority among processors that can receive the interrupt.
	When RH is 0, the interrupt is directed to the processor listed in the Destination ID field.
	When RH is 1 and the physical destination mode is used, the Destination ID field must not be set to FFH; it must point to a processor that is present and enabled to receive the interrupt.
	When RH is 1 and the logical destination mode is active in a system using a flat addressing model, the Destination ID field must be set so that bits set to 1 identify processors that are present and enabled to receive the interrupt.
	If RH is set to 1 and the logical destination mode is active in a system using cluster addressing model, then Destination ID field must not be set to FFH; the processors identified with this field must be present and enabled to receive the interrupt.
Destination mode (DM) — This bit indicates whether the Destination ID field should be interpreted as logical or physical APIC ID for delivery of the lowest priority interrupt.
	If RH is 1 and DM is 0, the Destination ID field is in physical destination mode and only the processor in the system that has the matching APIC ID is considered for delivery of that interrupt (this means no redirection).
	If RH is 1 and DM is 1, the Destination ID Field is interpreted as in logical destination mode and the redirection is limited to only those processors that are part of the logical group of processors based on the processor’s logical APIC ID and the Destination ID field in the message. The logical group of processors consists of those identified by matching the 8-bit Destination ID with the logical destination identified by the Destination Format Register and the Logical Destination Register in each local APIC. The details are similar to those described in Section 10.6.2, “Determining IPI Destination.”
	If RH is 0, then the DM bit is ignored and the message is sent ahead independent of whether the physical or logical destination mode is used.
*/
u32 msi_help_addr(int apicid)
{
	return 0xfee00000 | (apicid<<12);
}




/*
The layout of the Message Data Register is shown in Figure 10-25. Reserved fields are not assumed to be any value. Software must preserve their contents on writes. Other fields in the Message Data Register are described below.
Vector — This 8-bit field contains the interrupt vector associated with the message.
	Values range from 010H to 0FEH. Software must guarantee that the field is not programmed with vector 00H to 0FH.
Delivery Mode — This 3-bit field specifies how the interrupt receipt is handled. Delivery Modes operate only in conjunction with specified Trigger Modes. Correct Trigger Modes must be guaranteed by software. Restrictions are indicated below:
	000B (Fixed Mode) — Deliver the signal to all the agents listed in the destination. The Trigger Mode for fixed delivery mode can be edge or level.
	001B (Lowest Priority) — Deliver the signal to the agent that is executing at the lowest priority of all agents listed in the destination field. The trigger mode can be edge or level.
	010B (System Management Interrupt or SMI) — The delivery mode is edge only. For systems that rely on SMI semantics, the vector field is ignored but must be programmed to all zeroes for future compatibility.
	100B (NMI) — Deliver the signal to all the agents listed in the destination field. The vector information is ignored. NMI is an edge triggered interrupt regardless of the Trigger Mode Setting.
	101B (INIT) — Deliver this signal to all the agents listed in the destination field. The vector information is ignored. INIT is an edge triggered interrupt regardless of the Trigger Mode Setting.
	111B (ExtINT) — Deliver the signal to the INTR signal of all agents in the destination field (as an interrupt that originated from an 8259A compatible interrupt controller). The vector is supplied by the INTA cycle issued by the activation of the ExtINT. ExtINT is an edge triggered interrupt.
Level — Edge triggered interrupt messages are always interpreted as assert messages.
	For edge triggered interrupts this field is not used.
	For level triggered interrupts, this bit reflects the state of the interrupt input.
Trigger Mode — This field indicates the signal type that will trigger a message.
	0 — Indicates edge sensitive.
	1 — Indicates level sensitive.
*/
u32 msi_help_data(int vector, int islevel, int deassert)
{
	return (vector & 0xFF) | ((islevel==1) ? (1<<15) : 0) | (deassert == 1 ? 0 : (1 << 14));;
}
