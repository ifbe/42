#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
#include<linux/input.h>		//	/dev/input/event
#include<sys/epoll.h>		//	epoll




//epoll
int epfd=-1;			//the manager
struct epoll_event ev;

//device
int _dev_input_event[10];	//the workers
char devicename[32]={
	'/','d','e','v',		//[0,3]:	/dev
	'/','i','n','p','u','t',	//[4,9]:	/input
	'/','e','v','e','n','t',	//[10,15]:	/event
	'0',0				//[16,17]:	0x30,0x0
};




void waitevent(QWORD* first,QWORD* second)
{
	
}




void initevent()
{
	//create one file manager?
	epfd=epoll_create1(0);
	if(epfd==-1)
	{
		printf("epoll failed\n");
		exit(-1);
	}

	//open all events:	/dev/input/event%d
	int i,count=0;
	for(i=0;i<10;i++)
	{
		devicename[16]=0x30 + i;
		_dev_input_event[i]=open(devicename , O_RDONLY | O_NONBLOCK);
		if(_dev_input_event[i] == -1)
		{
			count=i;
			break;
		}
	}

	//manage them all
	for(i=0;i<count;i++)
	{
		ev.data.fd = _dev_input_event[i];
		ev.events = EPOLLIN | EPOLLET;
		epoll_ctl(epfd , EPOLL_CTL_ADD , _dev_input_event[i] , &ev);
	}
}
void killevent()
{
	int i=0;
	for(i=0;i<10;i++)
	{
		close(_dev_input_event[i]);
	}
	close(epfd);
}