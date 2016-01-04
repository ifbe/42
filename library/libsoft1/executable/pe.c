int ispe(char* addr)
{
	unsigned long long temp;

	//[0,1]:'M','Z'
	temp=*(unsigned short*)addr;
	if(temp!=0x5a4d)return 0;

	//[0x3c,0x3f]:	=0x40,=0xf0,=0x?
	temp=*(unsigned int*)(addr+0x3c);
	if(temp>=0xffc)return 0;

	temp=*(unsigned int*)(addr+temp);
	if(temp!=0x4550)return 0;

	return 64;
}
