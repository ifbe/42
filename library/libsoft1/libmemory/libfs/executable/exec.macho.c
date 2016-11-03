int ismacho(char* addr)
{
	unsigned int temp;

	//cafe babe ?
	temp=*(unsigned int*)addr;
	if(temp==0xbebafeca)return 1111;

	return 0;
}
