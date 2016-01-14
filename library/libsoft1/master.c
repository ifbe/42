#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void initmaster(char*);
void initservent(char*);
void masterlist(char*);
void masterinto(char*);




void initsoftware(char* world)
{
	//(仆人)最开始看着谁
	initmaster(world);
	initservent(world);

	//扫描一遍所有认识的东西，选中找到的第一个
	masterinto(0);
}
void killsoftware()
{
}
