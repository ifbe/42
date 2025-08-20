//
#define DR     0x00	//Data Register
#define RSRECR 0x04	//?
#define FR     0x18	//Flag register
#define ILPR   0x20	//not in use
#define IBRD   0x24	//Integer Baud rate divisor
#define FBRD   0x28	//Fractional Baud rate divisor
#define LCRH   0x2c	//Line Control register
#define CR     0x30	//Control register
#define IFLS   0x34	//Interrupt FIFO Level Select Register
#define IMSC   0x38	//Interrupt Mask Set Clear Register
#define RIS    0x3c	//Raw Interrupt Status Register
#define MIS    0x40	//Masked Interrupt Status Register
#define ICR    0x44	//Interrupt Clear Register
#define DMACR  0x48	//DMA Control Register
#define ITCR   0x80	//Test Control register
#define ITIP   0x84	//Integration test input reg
#define ITOP   0x88	//Integration test output reg
#define TDR    0x8c	//Test Data reg


int rpiuart_create()
{
	return 0;
}
int rpiuart_delete()
{
	return 0;
}
int rpiuart_reader()
{
	return 0;
}
int rpiuart_writer()
{
	return 0;
}
