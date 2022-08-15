#include "my-naming.h"
#include "my-struct.h"
#include "my-bignode.h"




//
u32 getrandom();
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
int memorydelete(void*);
void* memorycreate(int size, int flag);
//
int threaddelete(void*);
void* threadcreate(void*, void*);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int printmemory(void*, int);
int printmmio(void*, int);
int say(void*, ...);
