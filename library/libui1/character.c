//
void initascii(char*);
void initunicode(char*);
void initbackground(char*);
void initshape(char*);
//
void registermenu(char*);		//menu.c
void registerhex(char*);                //1.hex.c
void register2048(char*);               //2.2048.c
void registerkeyboard(char*);           //2.keyboard.c
void registertree(char*);               //2.tree.c
void registersketchpad(char*);          //3.sketchpad.c
void registerconsole(char*);            //4.console.c




int uicommand(char* p)
{
	return 0;	//ret<=0:failed
}




void initcharacter(char* addr)
{
	//clean everything
	int i;
	for(i=0;i<0x100000;i++)addr[i]=0;

	//words
	initascii(addr);
	initunicode(addr);

	//pictures
	initbackground(addr);
	initshape(addr);

	//[+0x00,0x3f]:		menu.c
	registermenu(addr);
	addr+=0x40;

	//[+0x40,+0x7f]:        1.hex.c
	registerhex(addr);
	addr += 0x40;

	//[+0x80,+0xbf]:        2.2048.c
	register2048(addr);
	addr += 0x40;

	//[+0xc0,+0xff]:        2.keyboard.c
	registerkeyboard(addr);
	addr += 0x40;

	//[+0x100,+0x13f]:      2.tree.c
	registertree(addr);
	addr += 0x40;

	//[+0x140,+0x17f]:      3.sketchpad.c
	registersketchpad(addr);
	addr += 0x40;

	//[+0x180,+0x1bf]:      4.console.c
	registerconsole(addr);
	addr += 0x40;
}
void killcharacter()
{
}
