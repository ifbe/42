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




static struct item* obj;
void initshell(void* addr)
{
	signal(SIGCHLD, SIG_IGN);
	obj = addr;
}
void freeshell()
{
}
void ptmxalloc()
{
}




int shell_insert()
{
	return 0;
}
int shell_remove()
{
	return 0;
}
int shell_search(char* p)
{
	int ret = system("ls /dev/pts/");
	return 0;
}
int shell_modify(char* p, int speed)
{
	return 0;
}




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




_obj* shell_create(char* p, int baud)
{
	int fd;
	int ret;
	char* name;

	fd = open("/dev/ptmx", O_RDWR);
	if(fd <= 0)
	{
		printf("error@open:%d\n",errno);
		return 0;
	}

	//obj
	struct item* oo = &obj[fd];
	oo->sockinfo.fd = fd;
	say("fd=%d,obj=%p\n", fd, oo);

	ret = grantpt(fd);
	if(ret < 0)
	{
		printf("error@grantpt:%d\n",errno);
		return 0;
	}

	ret = unlockpt(fd);
	if(ret < 0)
	{
		printf("error@unlockpt:%d\n",errno);
		return 0;
	}

	name = ptsname(fd);
	if(name == 0)
	{
		printf("error@ptsname:%d\n",errno);
		return 0;
	}
	printf("%.*s\n", 16, name);

	ret = fork();
	if(ret < 0)return 0;
	if(ret == 0)systemshell_child(name);

	kqueue_add(fd);

	return oo;
}
int shell_delete(_obj* oo)
{
	int fd = oo->sockinfo.fd;
	if(fd < 0)return 0;

	close(fd);
	return 0;
}
int shell_reader(_obj* oo,int xx, void* arg,int cmd, void* buf, int len)
{
	int fd = oo->sockinfo.fd;
	if(fd < 0)return 0;

	int ret = read(fd, buf, len);
	return ret;
}
int shell_writer(_obj* oo,int xx, void* arg,int cmd, void* buf, int len)
{
	int fd = oo->sockinfo.fd;
	if(fd < 0)return 0;

	int ret = write(fd, buf, len);
	return ret;
}




int shell_attach()
{
	return 0;
}
int shell_detach()
{
	return 0;
}
int shell_takeby(_obj* oo,int xx, void* arg,int cmd, void* buf, int len)
{
	return 0;
}
int shell_giveby(_obj* oo,int xx, void* arg,int cmd, void* buf, int len)
{
	return 0;
}