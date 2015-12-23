extern char* _binary_unicode_unicode_start;
void initbackground(char*);
void initshape(char*);
void initanscii(char*);




//todo
void initpixel(char* addr)
{
	initbackground(addr);
	initshape(addr);
	initanscii(addr);
}
unsigned char* whereisunicodetable()
{
	return (unsigned char*)&_binary_unicode_unicode_start;
}
