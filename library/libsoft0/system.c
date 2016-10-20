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
//socket
int createsocket();
int deletesocket();
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
		createfile();		//2
		createfolder();		//3
		//createprocess();	//4
		createrandom();		//5
		createsocket();		//6
	}
	say("[8,c):createed system\n");
}
void systemdelete()
{
	say("[8,c):deleteing system\n");

	deletesocket();		//6
	deleterandom();		//5
	//deleteprocess();	//4
	deletefolder();		//3
	deletefile();		//2
}
