#include "libuser.h"
struct reg{
	union{
	u64 x;
	u32 w;
	};
};
struct cpu{
	struct reg reg[30];

	union{
	u64 lr;
	};

	union{
	u64 xzr;
	u32 wzr;
	};

	u64 pc;
	u64 sp;

	u64 cspr;
	u64 cache;

	u32 code[0];
};




static void arm64_search(struct entity* act, u8* buf)
{
}
static void arm64_modify(struct entity* act, u8* buf)
{
}
static void arm64_delete(struct entity* act, u8* buf)
{
	//write cpustat to file

	//free 64m
}
static void arm64_create(struct entity* act, void* arg, int argc, u8** argv)
{
	//alloc 64m
	struct cpu* cpu = act->buf0 = memorycreate(0x100000, 0);
	if(0 == cpu)return;

	//default value
	cpu->pc = 0x80000;
	cpu->cache = 0xffffffff;

	//read cpustat from file
}




static void arm64_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void arm64_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_clk_ == foot){
		struct cpu* cpu = ent->buf0;
		if(0 == cpu)return;

		u64 where = cpu->pc & 0xfffffffffffff000;
		if(cpu->cache != where){
			say("cache miss, reading memory: [%llx,%llx]\n", where, where+0xfff);
			int ret = relationread(ent,_mmio_, stack,sp, 0,where, cpu->code,0x1000);
			if(ret <= 0)return;
		}
		cpu->cache = where;

		where = (cpu->pc&0xfff)>>2;
		say("%08x: %x\n", cpu->pc, cpu->code[where]);

		cpu->pc += 4;
	}
}
static void arm64_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void arm64_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void arm64_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('a','r','m','6','4', 0, 0, 0);

	p->oncreate = (void*)arm64_create;
	p->ondelete = (void*)arm64_delete;
	p->onsearch = (void*)arm64_search;
	p->onmodify = (void*)arm64_modify;

	p->onlinkup = (void*)arm64_linkup;
	p->ondiscon = (void*)arm64_discon;
	p->onread  = (void*)arm64_read;
	p->onwrite = (void*)arm64_write;
}
