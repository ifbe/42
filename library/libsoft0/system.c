#define QW unsigned long long
#define DW unsigned int
#define WORD unsigned short
#define BYTE unsigned char
//file
int createfile();
int deletefile();
int startfile();
int stopfile();
int listfile();
int choosefile();
int readfile( char* buf,QW sector,DW count);
int writefile(char* buf,QW sector,DW count);
//folder
int createfolder();
int deletefolder();
int startfolder(char* foldername);
int stopfolder();
int listfolder();
int choosefolder();
int readfolder(char* name);
int writefolder(char* name);
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
static int realtype=0;
int softevent(QW* first,QW* second)
{
	return 0;
}








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int systemlist(char* p)
{
	if(realtype==0)return listfolder(p);
	else if(realtype==1)return listfile(p);
}
int systemchoose(char* p)
{
	if(realtype==0)return choosefolder(p);
	else if(realtype==1)return choosefile(p);
}
int systemread(char* buf,QW sector,DW count)
{
	if(realtype==0)return readfolder(buf);
	else if(realtype==1)
	{
		return readfile(buf,sector,count);
	}
}
int systemwrite(char* buf,QW sector,DW count)
{
	if(realtype==0)return writefolder(buf);
	else if(realtype==1)return readfile(buf,sector,count);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
int systemstart(int type,char* p)
{
	int ret;
	realtype=type;

	if(realtype==0)ret=startfolder(p);
	else if(realtype==1)ret=startfile(p);

	//printf("=%d\n",ret);
	return ret;
}
int systemstop()
{
	if(realtype==0)return stopfolder();
	else if(realtype==1)return stopfile();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
