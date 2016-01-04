int is7z(char* addr)
{
	unsigned int temp;

	//[0,3]:	0x37,0x7a,0xbc,0xaf
	temp=*(unsigned int*)addr;
	if(temp!=0xafbc7a37)return 0;

	//[4,5]:	0x27,0x1c
	temp=*(unsigned short*)(addr+4);
	if(temp!=0x1c27)return 0;

	return 7;
}
