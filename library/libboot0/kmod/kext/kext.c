#include <mach/mach_types.h>
#include <libkern/libkern.h>
kern_return_t TestExtension_start(kmod_info_t * ki, void *d);
kern_return_t TestExtension_stop(kmod_info_t *ki, void *d);
kern_return_t TestExtension_start(kmod_info_t * ki, void *d)
{
    printf("TestExtension Start!\n");
    return KERN_SUCCESS;
}
kern_return_t TestExtension_stop(kmod_info_t *ki, void *d)
{
    printf("TestExtension Stop!\n");
    return KERN_SUCCESS;
}