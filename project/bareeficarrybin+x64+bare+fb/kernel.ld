ENTRY(efi_main)
phys = 0;
SECTIONS
{
.text :
{
    . = ALIGN(8);
    *(.text)
}
.rodata :
{
    . = ALIGN(8);
    *(.rodata)
}
.data :
{
    . = ALIGN(8);
    *(.data)
}
.bss :
{
    . = ALIGN(8);
    *(.bss)
}
end = .;
}
