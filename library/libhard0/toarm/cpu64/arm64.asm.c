
void haltwaitforint()
{
    asm("wfi");
}
void haltwaitforexc()
{
    asm("wfe");
}