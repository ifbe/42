#define u64 unsigned long long
void eventwrite(u64,u64,u64,u64);




static void sig_int(int a)
{
	eventwrite(0x3, 0x64626b, 0, 0);
}
static void sig_tstp(int a)
{
	eventwrite(0x1a, 0x64626b, 0, 0);
}




void createsignal()
{
}
void deletesignal()
{
}
