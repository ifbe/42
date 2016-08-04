#define QW unsigned long long
#define DW unsigned int
#define WORD unsigned short
#define BYTE unsigned char
//file
int initfile();
int killfile();
int startfile();
int stopfile();
int listfile();
int choosefile();
int readfile( char* buf,QW sector,DW count);
int writefile(char* buf,QW sector,DW count);
//folder
int initfolder();
int killfolder();
int startfolder(char* foldername);
int stopfolder();
int listfolder();
int choosefolder();
int readfolder(char* name);
int writefolder(char* name);
//process
int initprocess();
int killprocess();
//random
int initrandom();
int killrandom();
//socket
int initsocket();
int killsocket();
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
void systeminit(char* module,char* addr)
{
	if(module==0)
	{
		initfile();		//2
		initfolder();		//3
		//initprocess();	//4
		initrandom();		//5
		initsocket();		//6
	}
	say("[8,c):inited system\n");
}
void systemkill()
{
	say("[8,c):killing system\n");

	killsocket();		//6
	killrandom();		//5
	//killprocess();	//4
	killfolder();		//3
	killfile();		//2
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
