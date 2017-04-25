#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct tcp
{
	u16 srcport;
	u16 dstport;
	u32 sequence;
	u32 acknowledge;
	u16 headflag;
	u16 winsize;
	u16 checksum;
	u16 urgent;
};




void tcp_read_syn()
{
}
void tcp_read_ack()
{
}
void tcp_read()
{
}




void tcp_write_syn()
{
}
void tcp_write_ack()
{
}
void tcp_write()
{
}
