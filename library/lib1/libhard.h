#include "my-mathdata.h"
#include "my-naming.h"
#include "my-struct.h"

#include "my-bignode.h"
#include "my-bigwire.h"




void* eventwrite(u64,u64,u64,u64);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
u64 dateread();
u64 timeread_ns();
u64 timeread_us();
u64 timeread_ms();
int sleep_ns(int);
int sleep_us(int);
int sleep_ms(int);
//
void* memorycreate(int, int);
int memorydelete(void*);
//
int printmemory(void*, int);
int printmmio(void*, int);
//
int say(void*, ...);
int dbg(void*, ...);