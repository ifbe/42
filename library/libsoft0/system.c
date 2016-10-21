#define QW unsigned long long
#define DW unsigned int
#define u16 unsigned short
#define u8 unsigned char
//file
int createfile();
int deletefile();
//folder
int createfolder();
int deletefolder();
//process
int createprocess();
int deleteprocess();
//random
int createrandom();
int deleterandom();
//signal
int createsignal();
int deletesignal();
//
void printmemory(char*,int);
void say(char*,...);




//
int softevent(QW* what, QW* who, QW* where, QW* when)
{
	return 0;
}




void systemcreate(char* module,char* addr)
{
	if(module==0)
	{
		createfile();
		createfolder();
		createrandom();
		createsignal();
	}
	say("[8,c):createed system\n");
}
void systemdelete()
{
	say("[8,c):deleteing system\n");

	deletesignal();
	deleterandom();
	deletefolder();
	deletefile();
}
