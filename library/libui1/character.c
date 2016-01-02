void initbackground(char*);
void initshape(char*);
void initanscii(char*);
void initunicode(char*);




//todo
void initcharacter(char* addr)
{
	//
	initbackground(addr);
	initshape(addr);
	initanscii(addr);
	initunicode(addr);
}
void killcharacter()
{
}
