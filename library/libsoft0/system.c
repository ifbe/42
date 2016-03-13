#define QW unsigned long long
#define DW unsigned int
#define WORD unsigned short
#define BYTE unsigned char
//event
void initevent();
void killevent();
//file
void initfile();
void killfile();
void readfile(char* buf,QW sector,QW disk,DW count);
//process
void initprocess();
void killprocess();
//random
void initrandom();
void killrandom();
//socket
void initsocket();
void killsocket();




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static int lowertype=0;
void systeminit(char* type,char* addr)
{
	if(type==0)
	{
		initevent();		//1
		initfile();			//2
		//initprocess();	//4
		initrandom();		//5
		//initsocket();		//6
	}
}
void systemkill()
{
	//killsocket();		//6
	killrandom();		//5
	//killprocess();	//4
	killfile();			//2
	killevent();		//1
}
void systemopen()
{
}
void systemclose()
{
}
void systemlist()
{
}
void systemswitch()
{
}
void systemread(char* buf,QW sector,QW disk,DW count)
{
	readfile(buf,sector,disk,count);
}
void systemwrite(char* buf,QW sector,QW disk,DW count)
{
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
	systemread(rdi,rsi,0,rcx);
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
