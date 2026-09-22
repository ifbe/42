#include <stdio.h>
#include <stdlib.h>
#include "libboot.h"
void setentryandstack(void* entry, void* stack);
void example_unix(int, char**);




int main(int argc, char** argv)
{
	u8 tmp;
	setentryandstack(main, &tmp);

	example_unix(argc, argv);

	return 0;
}
