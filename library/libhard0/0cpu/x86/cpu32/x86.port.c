unsigned char in8( unsigned short port )
{
    unsigned char ret;
    asm volatile( "inb %1, %0"
	: "=a"(ret)
	: "Nd"(port) );
    return ret;
}
unsigned short in16( unsigned short port )
{
    unsigned short ret;
    asm volatile( "inw %1, %0"
	: "=a"(ret)
	: "Nd"(port) );
    return ret;
}
unsigned int in32( unsigned short port )
{
    unsigned int ret;
    asm volatile( "inl %1, %0"
	: "=a"(ret)
	: "Nd"(port) );
    return ret;
}




void out8( unsigned short port, unsigned char val )
{
    asm volatile( "outb %0, %1"
	:
	: "a"(val), "Nd"(port) );
}
void out16( unsigned short port, unsigned short val )
{
    asm volatile( "outw %0, %1"
	:
	: "a"(val), "Nd"(port) );
}
void out32( unsigned short port, unsigned int val )
{
    asm volatile( "outl %0, %1"
	:
	: "a"(val), "Nd"(port) );
}
