#include "my-naming.h"
#include "my-struct.h"
#include "my-bignode.h"
#include "my-bigwire.h"




//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
u64 timeread_us();
u64 dateread();
//
void* memorycreate(int, int);
int memorydelete(void*);
//
int printmemory(void*, int);
int printmmio(void*, int);
//
int say(void*, ...);
int dbg(void*, ...);