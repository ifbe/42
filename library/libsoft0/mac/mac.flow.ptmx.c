#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "libsoft.h"
char* ptsname(int);
int grantpt(int);
int unlockpt(int);
int kqueue_add(int);




static struct sysobj* obj;
void systemshell_child(char* p)
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
	execl("/system/bin/sh", "/system/bin/sh", NULL);
}




int readshell(int fd, int off, char* buf, int len)
{
	int ret;
	ret = read(fd, buf, len);
	return ret;
}
int writeshell(int fd, int off, char* buf, int len)
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
int stopshell(int fd)
{
	close(fd);
	return 0;
}
int startshell(char* p, int baud)
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
	else if(ret == 0)systemshell_child(name);

	kqueue_add(fd);
	return fd;
}




void freeshell()
{
}
void initshell(void* addr)
{
	signal(SIGCHLD, SIG_IGN);
	obj = addr;
}
