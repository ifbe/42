void say(void*, ...);




int check_macho(char* addr)
{
	unsigned int temp;

	//cafe babe ?
	temp=*(unsigned int*)addr;
	if(temp==0xbebafeca)return 1111;

	return 0;
}
int parse_macho(char* addr)
{
	say("macho\n");
	return 0;
}
