#include "const/def.h"
#include "const/math.h"
#include "const/naming.h"
#include "const/event.h"
#include "data/node-inner.h"
#include "data/node-outer.h"
#include "data/wire-inner.h"
#include "data/wire-outer.h"




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
void* memoryalloc(int, int);
int memoryfree(void*);
//
int printmemory(void*, int);
int printmmio(void*, int);
//
int logtoall(void*, ...);
int dbg(void*, ...);
