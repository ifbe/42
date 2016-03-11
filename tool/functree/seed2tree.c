#include <stdio.h>  
#include <stdlib.h>  
#include <string.h> 
#include <dirent.h>  
#include <unistd.h>  
#include <fcntl.h>
#include <sys/stat.h>  
#include <sys/types.h> 
static int dest=-1;
static int src=-1;
char* wantedfuncname="main";




//在已经读完的缓冲区里搜索自己
//搜不到就写上等级(tab个数)，以及名称(字符串)，外加问号
//搜到就递归调用这个函数(省的手动压栈)
static char tempforname[256];
static int namestack[256];
static int depth=0;
void nodeorleaf
(
	char* start,int count,		//search where , byte count
	char* wantname,int namesize	//name addr , name count
)
{
	char ch1=0;
	int new=1;	//isnewline
	int this=0,run=0;

	//try to find it!
	while(1)
	{
		if(new)
		{
			//不一样就提前跳出来
			for(run=0;run<namesize;run++)
			{
				if(start[this+run] != wantname[run])break;
			}

			//没有提前跳出来说明找到了那就跳出while(1)
			if( run==namesize )
			{
				ch1=start[this+run];
				//必须再确认一个东西,防止printf找到printfxxxx
				if( (ch1=='\n' ) | (ch1==0x9) | (ch1==0x20) )
				{
					break;
				}
			}

			//继续往下搜索
			else new=0;
		}
		else		//old
		{
			if(start[this]=='\n')
			{
				new=1;
			}
		}

		//无论if或者else都得到这里进行下一次检查
		this++;
		if( this >= count )break;
	}
	//不管怎样，进来了都要打上名字
	for(run=0;run<depth;run++)
	{
		printf("	");
		write(dest,"	",1);
	}
	for(run=0;run<namesize;run++)
	{
		printf( "%c" , wantname[ run ] );
		write( dest , wantname+run , 1 );
	}




	//1.第1种：找不到了
	if( this >= count )
	{
		printf("\n");
		write(dest,"\n",1);
		return;
	}

	//2.第1种：判断递归
	if( depth>0)
	{
		if( namestack[depth-1]==this )
		{
			printf("	#recursion\n");
			write(dest,"	#recursion\n",12);
			return;
		}
	}

	//3.第2种：循环调用

	//4.第3种：栈太深不敢进
	if(depth>=8)
	{
		printf("	#too deep!\n");
		write(dest,"	#too deep!\n",12);
		return;
	}




	//左括号
	printf("	@%x\n" , this );
	write(dest,"\n",1);
	for(run=0;run<depth;run++)
	{
		printf("	");
		write(dest,"	",1);
	}
	printf("{\n");
	write(dest,"{\n",2);

	//进去
	run=this;
	while(1)
	{
		if( start[run] == '}' )break;
		if(start[run]==0x9)
		{
			new=0;
			while(start[1+run+new]!='\n')new++;

			namestack[depth]=this;
			depth++;
			nodeorleaf(start,count,start+run+1,new);
			depth--;

			run+=new;
		}

		run++;
	}

	//右括号
	for(run=0;run<depth;run++)
	{
		printf("	");
		write(dest,"	",1);
	}
	printf("}\n");
	write(dest,"}\n",2);
}
void seed2tree(char* inininin)
{
	struct stat	statbuf;
	char* buf;
	int temp;
	printf("%s\n",inininin);

	//check
	temp=stat( inininin , &statbuf );
	if(temp == -1)
	{
		printf("wrong file\n");
		goto statfailed;
	}

	//open
	src=open(inininin , O_RDONLY);
	if(src<0)
	{
		printf("open failed\n");
		goto openfailed;
	}

	//malloc
	buf=(char*)malloc( (statbuf.st_size) + 0x1000 );
	if(buf==NULL)
	{
		printf("too large\n");
		goto mallocfailed;
	}

	//read
	temp=read(src,buf,statbuf.st_size);
	if(temp!=statbuf.st_size)
	{
		printf("read failed\n");
		goto readfailed;
	}

	//process
	depth=0;
	namestack[0]=0;
	nodeorleaf( buf , statbuf.st_size , wantedfuncname , strlen(wantedfuncname) );
/*
	for( temp=0;temp<statbuf.st_size;temp++ )
	{
		printf("%c",buf[temp]);
	}
*/

readfailed:
	free(buf);
mallocfailed:
	close(src);
openfailed:
	return;
statfailed:
	return;
}




int main(int argc,char *argv[])  
{
	//
	if(argc==2)wantedfuncname=argv[1];

	//open,process,close
	dest=open("func.tree",O_CREAT|O_RDWR|O_TRUNC,S_IRWXU|S_IRWXG|S_IRWXO);
	seed2tree( "func.seed" );
	close(dest);

}
