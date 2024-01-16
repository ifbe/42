void logtoall(void*, ...);




int check_gz(unsigned char* addr)	//unsigned because addr[1]=0x8b
{
	if(addr[0] != 0x1f)return 0;
	if(addr[1] != 0x8b)return 0;
	if(addr[2] != 0x08)return 0;

	return 1;
}
int parse_gz(unsigned char* addr)
{
	logtoall("gz\n");
	return 0;
}
