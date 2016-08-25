/*
1:	add name in library/libui1/makefile
2:	add code in character.c character_create() and character_delete()
3:	replace "example" to "whatname"----->		:9,$s/example/whatname/g
4:	do your logic in this code
*/




#define u64 unsigned long long
#define u32 unsigned int
//
void backgroundcolor();
//
void say(char*,...);




static struct temp{
        u64 type;
        u64 id;
        u64 start;
        u64 end;

        u64 pixelbuffer;
        u64 pixelformat;
        u64 width;
        u64 height;
}*haha;




static void example_read_html()
{
}
static void example_read_pixel()
{
}
static void example_read_text()
{
}
static void example_read()
{
	//text
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		example_read_text();
	}

	//html
	else if( ( (haha->pixelformat)&0xffffffff) == 0x6c6d7468)
	{
		example_read_html();
	}

	//pixel
	else
	{
		example_read_pixel();
	}
}




static void example_write(u64* type, u64* key)
{
}




static void example_list()
{
}
static void example_change()
{
}




static void example_start()
{
}
static void example_stop()
{
}
void example_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x74736574;
	this[1] = 0x656c706d617865;

	this[10]=(u64)example_start;
	this[11]=(u64)example_stop;
	this[12]=(u64)example_list;
	this[13]=(u64)example_change;
	this[14]=(u64)example_read;
	this[15]=(u64)example_write;
}
void example_delete()
{
}
