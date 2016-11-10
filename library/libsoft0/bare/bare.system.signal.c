#define u64 unsigned long long
void eventwrite(u64,u64);




static void sig_int(int a)
{
	eventwrite(0x3, 0x64626b);
}
static void sig_tstp(int a)
{
	eventwrite(0x1a, 0x64626b);
}




void createsignal()
{
}
void deletesignal()
{
}
