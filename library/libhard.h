#include "naming.h"
#include "struct.h"
#include "operate.h"




void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void printmemory(void*, int);
void printmmio(void*, int);
void say(void*, ...);
void dbg(void*, ...);