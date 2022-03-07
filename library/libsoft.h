#include "my-naming.h"
#include "my-struct.h"
#include "my-bignode.h"




void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
//
double getsin(double);
double getcos(double);
double arcsin(double);
double arccos(double);
double arctanyx(double, double);
//
_obj* file_create(void*, int);
int file_delete(_obj*);
int file_take(_obj*, int, void*, int, void*, int);
int file_give(_obj*, int, void*, int, void*, int);
//
int readfolder( void*, int, void*, int, void*, int);
int writefolder(void*, int, void*, int, void*, int);
//
int threaddelete(u64);
u64 threadcreate(void*, void*);
int memorydelete(void*);
void* memorycreate(int, int);
void* eventread();
void* eventwrite(u64,u64,u64,u64);
//
u32 getrandom();
u64 dateread();
u64 timeread();
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
int mysnprintf(void*, int, void*, ...);
//
int printmemory(void*, int);
int printmmmio(void*, int);
int say(void*, ...);
