#include "libsoft.h"




struct saved_register{
	u64 rax;
	u64 rbx;
	u64 rcx;
	u64 rdx;
	u64 rsi;
	u64 rdi;

	u64 ip;
	u64 cs;
	u64 flag;
	u64 sp;
	u64 ss;
}__attribute__((packed));

struct saved_pagetable{
}__attribute__((packed));

struct saved_vmcb{
}__attribute__((packed));

struct savedstate{
	struct saved_register reg;
	struct saved_pagetable pt;
	struct saved_vmcb vmcb;
}__attribute__((packed));




int readprocess()
{
	return 0;
}
int writeprocess()
{
	return 0;
}
int startprocess()
{
	return 0;
}
int stopprocess()
{
	return 0;
}
int createprocess()
{
	return 0;
}
int deleteprocess()
{
	return 0;
}
