#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
int command(char* buffer);
int compare(char* first,char* second);
int waitinput(char*);
int worldandmemory();
int cleanall();




static char buffer[128];//键盘输入专用
int main()
{
	worldandmemory();

	while(1)
	{
		//1.等输入，再把这段里面所有的0x20变成0
		waitinput(buffer);

		if(compare(buffer, "exit" ) == 0)break;
		else command(buffer);
	}

	cleanall();
	return 0;
}
