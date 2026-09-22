#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <direct.h>
#include <windows.h>
#include "libboot.h"
void setentryandstack(void* entry, void* stack);
void example_win(int, char**);




int main(int argc, char** argv)
{
	u8 tmp;
	setentryandstack(main, &tmp);

	example_win(argc, argv);

	return 0;
}
