void logtoall(void*, ...);




int check_tar(char* addr)
{
	unsigned int temp;

	//[0x101]:	'u'
	if(addr[0x101] != 0x75)return 0;

	//[0x102,0x105]:	's','t','a','r'
	temp=*(unsigned int*)(addr+0x102);
	if(temp!=0x72617473)return 0;

	return 1111;
}
int parse_tar(char* addr)
{
	logtoall("tar\n");
	return 0;
}
