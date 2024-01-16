void logtoall(void*, ...);




int check_zip(char* addr)
{
	unsigned int temp;

	//0x50,0x4b,0x3,0x4
	temp=*(unsigned int*)addr;
	if(temp==0x04034b50)return 11111;

	return 0;
}
int parse_zip(char* addr)
{
	logtoall("zip\n");
	return 0;
}
