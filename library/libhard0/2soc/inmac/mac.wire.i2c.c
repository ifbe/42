#include "libsoft.h"



void* i2c_alloc()
{
    return 0;
}
int i2c_free()
{
    return 0;
}



int i2c_delete()
{
    return 0;
}
int i2c_create(u8* name, int flag, int argc, u8** argv)
{
    logtoall("@i2c_create:%s\n", name);
    return 0;
}
int i2c_read(int fd, int addr, u8* buf, int len)
{
    return 0;
}
int i2c_write(int fd, int addr, u8* buf, int len)
{
    return 0;
}