#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
static BYTE buffer[128];//键盘输入专用

void command(char* buffer);
void initmaster(QWORD addr);
int compare(char* first,char* second);

void waitinput(char* buffer);
QWORD whereisworld();




void main()
{
	initmaster(whereisworld());
	while(1)
	{
		//1.等输入，再把这段里面所有的0x20变成0
		waitinput(buffer);

		if(compare(buffer, "exit" ) == 0)break;
		else command(buffer);
	}
}
