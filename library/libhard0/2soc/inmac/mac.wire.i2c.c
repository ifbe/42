#include "libsoft.h"




void i2c_read(int fd, int addr, u8* buf, int len)
{
}
void i2c_write(int fd, int addr, u8* buf, int len)
{
}
void i2c_discon()
{
}
void i2c_linkup()
{
}




void i2c_list()
{
}
void i2c_choose()
{
}
void i2c_delete()
{
}
int i2c_create(u8* name, int flag, int argc, u8** argv)
{
    logtoall("@i2c_create:%s\n", name);
    return 0;
}
