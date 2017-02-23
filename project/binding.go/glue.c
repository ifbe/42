#include <dlfcn.h>

int do_birth()
{
    void* handle;
    typedef int (*FPTR)();

    handle = dlopen("./42.so", 1);
    FPTR fptr = (FPTR)dlsym(handle, "birth");

    int result = (*fptr)();
    return result;
}
int do_death()
{
    void* handle;
    typedef int (*FPTR)();

    handle = dlopen("./42.so", 1);
    FPTR fptr = (FPTR)dlsym(handle, "death");

    int result = (*fptr)();
    return result;
}
