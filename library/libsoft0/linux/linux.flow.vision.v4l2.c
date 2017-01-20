#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void eventwrite(u64,u64,u64,u64);
void printmemory(void*, int);
void say(void*, ...);




//
static struct buffer{
	void *start;
	char padding[8- sizeof(void*)];

	int length;
	int status;
}my[24];
//
static pthread_t id;
static int alive = 1;




void listvision()
{
}
void choosevision()
{
}
int readvision(char* buf, int frame)
{
}
int writevision(char* buf, int frame)
{
}
void* visionlistener(void* p)
{
	//v4l2_open
	int j,k;
	int fd = open("/dev/video0",O_RDWR);    //|O_NONBLOCK);
	if(fd <= 0)
	{
		printf("error@open /dev/video0\n");
		return 0;
	}

	//v4l2_capability
	struct v4l2_capability cap;
	if(-1==ioctl(fd,VIDIOC_QUERYCAP,&cap))
	{
		printf("VIDIOC_QUERYCAP error\n");
		return 0;
	}
	printf("v4l2_capability:\n");
	printf("	driver: %s\n", cap.driver);
	printf("	card: %s\n", cap.card);
	printf("	bus_info: %s\n", cap.bus_info);
	printf("	version: %08X\n", cap.version);
	printf("	capabilities: %08X\n", cap.capabilities);

	//v4l2_fmtdesc
	struct v4l2_fmtdesc desc;
	desc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	desc.index=0;
	printf("v4l2_fmtdesc:\n");
	while(ioctl(fd, VIDIOC_ENUM_FMT, &desc) != -1)
	{
		printf("	%s\n", desc.description);
		desc.index++;
	}
	if (desc.index == 0)
	{
		perror("VIDIOC_ENUM_FMT");
	}

	//v4l2_format
	struct v4l2_format fmt;
	fmt.type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	fmt.fmt.pix.width       = 640;
	fmt.fmt.pix.height      = 480;
	fmt.fmt.pix.pixelformat =
		V4L2_PIX_FMT_YUYV;
		//V4L2_PIX_FMT_MJPEG;
		//V4L2_PIX_FMT_SBGGR8;
		//V4L2_PIX_FMT_SN9C10X;
		//V4L2_PIX_FMT_MJPEG;
		//V4L2_PIX_FMT_JPEG;
		//V4L2_PIX_FMT_RGB24;
		//V4L2_PIX_FMT_UYVY;
		//V4L2_PIX_FMT_YUV422P;
		//V4L2_PIX_FMT_YUV420;
	if(ioctl(fd,VIDIOC_S_FMT,&fmt) == -1)
	{
		printf("VIDIOC_S_FMT error\n");
		return 0;
	}

	//v4l2_requestbuffers
	struct v4l2_requestbuffers req;
	req.count       = 24;
	req.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory      = V4L2_MEMORY_MMAP;     //V4L2_MEMORY_USERPTR;
	if(ioctl(fd,VIDIOC_REQBUFS,&req) == -1)
	{
		printf("VIDIOC_REQBUFS error\n");
		return 0;
	}

	//prepare
	struct v4l2_buffer buf;
	for(j=0;j<24;j++)
	{
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = j;
		if(ioctl(fd,VIDIOC_QUERYBUF,&buf) == -1)
		{
			printf("VIDIOC_QUERYBUF\n");
			return 0;
		}

		my[j].length = buf.length;
		my[j].start = mmap(
			NULL,
			buf.length,
			PROT_READ|PROT_WRITE,
			MAP_SHARED,
			fd,
			buf.m.offset
		);

		if(my[j].start == MAP_FAILED)
		{
			printf("fail@mmap\n");
			return 0;
		}
		if(ioctl(fd,VIDIOC_QBUF,&buf) == -1)
		{
			printf("VIDEOC_QBUF\n");
			return 0;
		}
	}

	//start
	j = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(ioctl(fd, VIDIOC_STREAMON, &j) == -1)
	{
		printf("error@ON\n");
		return 0;
	}

	//record
	int epfd;
	struct epoll_event ev;
	epfd=epoll_create(256);
	ev.data.fd=fd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
	j=0;
	while(alive)
	{
		//!!!!!!!!!!!!!!must take out ontime!!!!!!!!!!!!!!
		k = epoll_wait(epfd, &ev, 1, -1);
		if(k == -1)
		{
			printf("epoll error\n");
			return 0;
		}

		//deq
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		ioctl(fd, VIDIOC_DQBUF, &buf);

		//do
		//printmemory(my[j].start+0xfff, 16);
		eventwrite((u64)my[j].start, 'v', 0, 0);

		//enq
		ioctl(fd, VIDIOC_QBUF, &buf);
		j = (j+1)%24;
	}

	//stop
	j = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(ioctl(fd, VIDIOC_STREAMOFF, &j) == -1)
	{
		printf("error@OFF\n");
	}
	for(j=0;j<24;j++)
	{
		munmap(my[j].start, my[j].length);
	}

	//v4l2_release
	close(fd);
}




int startvision()
{
	alive = 1;
	pthread_create(&id, NULL, visionlistener, NULL);
	return 1;
}
void stopvision()
{
	alive = 0;
}
void createvision()
{
}
void deletevision()
{
}
