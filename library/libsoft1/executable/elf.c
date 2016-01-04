int iself(char* addr)
{
	unsigned int temp=*(unsigned int*)addr;
	if(temp==0x464c457f)return 0x666c65;
	return 0;
}
