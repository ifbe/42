#include "libhard.h"
//vpid
//ept
struct vmcs{
    //[0,4]
    u32 revision:31;
    u32 shadow:1;
    //[4,7]
    u32 exitcode;
    //[8,?]
}__attribute__((packed));




void vmresume(void* vmcs)
{
}
void vmlaunch(void* vmcs)
{
}




void vmxoff()
{
}
void vmxon()
{
}