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
int do_say()
{
    void* handle;
    typedef int (*FPTR)(void*, ...);

    handle = dlopen("./42.so", 1);
    FPTR fptr = (FPTR)dlsym(handle, "say");

    int result = (*fptr)("hello %d\n", 666666);
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
