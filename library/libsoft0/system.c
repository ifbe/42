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
int switchfile();
int readfile( char* buf,QW sector,DW count);
int writefile(char* buf,QW sector,DW count);
//folder
int initfolder();
int killfolder();
int startfolder(char* foldername);
int stopfolder();
int listfolder();
int switchfolder();
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
int systemswitch(char* p)
{
	if(realtype==0)return switchfolder(p);
	else if(realtype==1)return switchfile(p);
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








/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                |[want     ,     want+1m]|
                |                        | [where,]    //不要
 [where, ]      |                        |             //不要
            [---|--where,--]             |             //要后面
            [---|--where,----------------|----]        //要中间
                |  [where,    ]          |             //全要
                |  [---where,------------|----]        //要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(设备这一小块是逻辑上的哪) -> (内存这一小块想要逻辑上的哪)
(扇区，数量，是哪) -> (内存，数量，要哪)
*/
void cleverread(QW src,QW count,QW where  ,  BYTE* dst,QW size,QW want)
{
	BYTE* rdi=0;    //关键:读到哪儿
	QW rsi=0;    //读哪号扇区
	QW rcx=0;    //读几个扇区

	//改rdi,rsi,rcx数值
	if(where<want)             //3和4
	{
		rdi=dst;
		rsi=src + (want-where)/0x200;
		if(where+count*0x200<=want+size)
		{
			rcx=count-(want-where)/0x200;
		}
		else
		{
			rcx=size/0x200;
		}
	}
	else
	{
		rdi=dst + (where-want);
		rsi=src;
		if(where+count*0x200<=want+size)
		{
			rcx=count;
		}
		else
		{
			rcx=(want+size-where)/0x200;
		}
	}

/*
	say(
		"(%llx,%llx,%llx)->(%llx,%llx,%llx)\n",
		src,count,where,    dst,size,want
	);
	say(
		"rdi=%llx,rsi=%llx,rcx=%llx\n",
		rdi,rsi,rcx
	);
*/
	systemread(rdi,rsi,rcx);
}




/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                |[want     ,     want+1m]|
                |                        | [where,]    //不要
 [where, ]      |                        |             //不要
            [---|--where,--]             |             //要后面
            [---|--where,----------------|----]        //要中间
                |  [where,    ]          |             //全要
                |  [---where,------------|----]        //要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(内存这一小块是逻辑上的哪) -> (设备这一小块想要逻辑上的哪)
(来源，数量，是哪) -> (目的，数量，要哪)
*/
void cleverwrite(BYTE* src,QW count,QW where  ,  QW dst,QW size,QW want)
{
}
