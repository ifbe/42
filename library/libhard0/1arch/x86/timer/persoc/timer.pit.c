#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
u64 rdtsc();
//
u8 in8(u16 port);
void out8(u16 port, u8 data);
//
void irqchip_enableirq(int chip, int pin, int apicid, int intvec);
void irqchip_disableirq(int chip, int pin);
//
void printmemory(void*, int);
void logtoall(void*, ...);




static u64 cnt = 0;
void isr_825x(void* p)
{
	//if(0==(cnt%1000))logtoall("cnt=%d\n",cnt);
	cnt += 1;
}
u64 pit8254_ms()
{
	return cnt;
}
u64 pit8254_us()
{
	return cnt*1000;
}
u64 pit8254_ns()
{
	return cnt*1000*1000;
}


int pit8254_check()
{
	//while(0 == cnt);
	//logtoall("8254 works, cnt=%x, tsc=%llx\n\n", cnt, rdtsc());

	u64 pit_t0 = cnt;
	u64 pit_tn = pit_t0 + 10;		//10ms
	u64 pit_now = 0;
	u64 tsc_t0 = rdtsc();
	u64 tsc_tn = tsc_t0 + (u64)10*1000*1000*1000;
	u64 tsc_now = 0;
	logtoall("waiting (irqsum>100) or (tsc>10g): irqsum=%lld,tsc=%lld\n", pit_t0, tsc_t0);
	while(1){
		pit_now = cnt;
		tsc_now = rdtsc();

		if(tsc_now > tsc_tn){
			logtoall("(maybe noirq)irqsum=%lld,tsc=%lld\n", pit_now, tsc_now);
			tsc_tn += (u64)10*1000*1000*1000;
		}
		if(pit_now >= pit_tn){
			logtoall("(look good)irqsum=%lld,tsc=%lld\n", pit_now, tsc_now);
			pit_tn += 10;
		}
		if(pit_now >= pit_t0 + 100){
			logtoall("time_delta=0.1s, tsc_delta=%lld, avgfreq=%lldmhz\n", tsc_now - tsc_t0, (tsc_now-tsc_t0)*10/1000/1000);
			goto good;
		}
		if(tsc_now > tsc_t0 + (u64)3*10*1000*1000*1000){
			logtoall("pit fail\n");
			goto fail;
		}
	}
good:
	return 1;
fail:
	return 0;
}
int init825x()
{
	logtoall("@init825x, cnt=%x, tsc=%llx\n", cnt, rdtsc());
	u32 t = 3579545 / 3 / 1000;
	u16 lo = t & 0xff;
	u16 hi = t >> 8;

	out8(0x43, 0x34);
	out8(0x40, lo);
	out8(0x40, hi);

	irqchip_enableirq(0,0, 0,0x20);

	int ret = pit8254_check();

	logtoall("initpit end\n\n");
	return ret;
}
