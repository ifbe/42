#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>
#include<termios.h>
#include<sys/ioctl.h>
#include"system.h"
char* ptsname(int);
int grantpt(int);
int unlockpt(int);
int epoll_add(int);




static struct object* obj;




void systemshell_process(char* p)
{
	int ret;
	int slave = open(p, O_RDWR);
	if(slave == 0)
	{
		printf("error@open:%d\n",errno);
		return;
	}
	ret = ioctl(slave, TIOCSCTTY, NULL);

	setsid();
	dup2(slave, 0);
	dup2(slave, 1);
	dup2(slave, 2);
	execl("/bin/bash", "/bin/bash", NULL);
}
int readshell(int fd, char* buf, int off, int len)
{
	int ret;
	ret = read(fd, buf, len);
	return ret;
}
int writeshell(int fd, char* buf, int off, int len)
{
	int ret;
	ret = write(fd, buf, len);
	return ret;
}
int listshell(char* p)
{
	int ret = system("ls /dev/pts/");
	return 0;
}
int changeshell(char* p, int speed)
{
	return 0;
}
int stopshell()
{
	return 0;
}
int startshell(char* p)
{
	int fd;
	int ret;
	char* name;

	fd = open("/dev/ptmx", O_RDWR);
	if(fd <= 0)
	{
		printf("error@open:%d\n",errno);
		return -1;
	}

	ret = grantpt(fd);
	if(ret < 0)
	{
		printf("error@grantpt:%d\n",errno);
		return -2;
	}

	ret = unlockpt(fd);
	if(ret < 0)
	{
		printf("error@unlockpt:%d\n",errno);
		return -3;
	}

	name = ptsname(fd);
	if(name == 0)
	{
		printf("error@ptsname:%d\n",errno);
		return -4;
	}
	printf("%.*s\n", 16, name);

	ret = fork();
	if(ret < 0)return -5;
	else if(ret == 0)systemshell_process(name);
	else
	{
/*
		if(info.buf == 0)
		{
			info.enq = 0;
			info.deq = 0;
			info.len = 0x100000;
			info.buf = (void*)malloc(info.len);
		}
		startthread(systemshell_thread, 0);
*/
		write(fd, "unset PROMPT_COMMAND\n", 21);
		obj[fd].type_sock = hex32('u','a','r','t');
		obj[fd].info.enq = 0;
		obj[fd].info.deq = 0;
		obj[fd].info.len = 0x100000;
		obj[fd].info.buf = (void*)malloc(0x100000);
		epoll_add(fd);
		return fd;
	}
	return 0;
}
void createshell(void* addr)
{
	signal(SIGCHLD, SIG_IGN);
	obj = addr;
}
void deleteshell()
{
}
