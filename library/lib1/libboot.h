#include "const/def.h"
#include "const/math.h"
#include "const/naming.h"
#include "data/node-inner.h"
#include "data/node-outer.h"
#include "data/wire-inner.h"
#include "data/wire-outer.h"




//
u32 random_read();
//
int sleep_ms(int);
int sleep_us(int);
int sleep_ns(int);
//
u64 dateread();
u64 timeread_us();
//
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
int memoryfree(void*);
void* memoryalloc(int size, int flag);
//
int threaddelete(void*);
void* threadcreate(void*, void*);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int printmemory(void*, int);
int printmmio(void*, int);
int logtoall(void*, ...);
