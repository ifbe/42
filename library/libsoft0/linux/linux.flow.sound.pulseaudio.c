#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void eventwrite(u64,u64,u64,u64);
void printmemory(void*, int);
void say(void*, ...);




//
static pthread_t id;
static int alive = 1;
static int freq;
static int channel;
