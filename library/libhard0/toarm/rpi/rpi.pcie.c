#include "libhard.h"
#define PCIE_HOST_BASE 0xFD500000
#define PCIE_HOST_END  (PCIE_HOST_BASE + 0x930F)




void brcmpcie_init()
{
    say("@brcmpcie_init\n");
    printmmio((void*)PCIE_HOST_BASE, 0x40);
}
void brcmpcie_exit()
{
}