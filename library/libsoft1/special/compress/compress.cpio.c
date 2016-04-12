int iscpio(char* addr)
{
	unsigned long long temp;

	//[0,3]:	0x30,0x37,0x30,0x37
	temp=*(unsigned int*)addr;
	if(temp != 0x37303730)return 0;

	//addr+4:	0x30
	if(addr[4] != 0x30)return 0;

	//0x37="standard ASCII format"
	//0x31="new ASCII(aka SVR4) format"
	//0x32="CRC format"
	if(addr[5] == 0x37)return 7;
	if(addr[5] == 0x31)return 1;
	if(addr[5] == 0x32)return 2;

	return 0;
}
