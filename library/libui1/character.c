void initbackground(char*);
void initshape(char*);
void initascii(char*);
void initunicode(char*);




//todo
void initcharacter(char* addr)
{
	//
	initbackground(addr);
	initshape(addr);
	initascii(addr);
	initunicode(addr);
}
void killcharacter()
{
}
