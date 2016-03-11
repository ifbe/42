#include<stdio.h>
void openfolder(char*);
void listfolder();
int main(int argc,char* argv[])
{
	int i;
	char name[256];

	if(argc==1)openfolder(".");
	else openfolder(argv[1]);
	listfolder();
}
