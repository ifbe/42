#include "libboot.h"



void* style_alloc()
{
    return memory_alloc(sizeof(struct style));
}
void style_recycle()
{
}