#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include "arena.h"




//
struct buffer{
	union{
		u64 addr;
		void* buf;
	};
	u64 len;
	u64 width;
	u64 height;
};
static struct buffer info[24];
static int cur = 0;
static int alive = 1;




void* visionlistener(void* p)
{
	//v4l2_open
	int j;
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

		info[j].width = 640;
		info[j].height = 480;
		info[j].len = buf.length;
		info[j].buf = mmap(
			NULL,
			buf.length,
			PROT_READ|PROT_WRITE,
			MAP_SHARED,
			fd,
			buf.m.offset
		);

		if(info[j].buf == MAP_FAILED)
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
	epfd = epoll_create(256);
	ev.data.fd=fd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
	cur = 0;
	while(alive)
	{
		//!!!!!!!!!!!!!!must take out ontime!!!!!!!!!!!!!!
		j = epoll_wait(epfd, &ev, 1, -1);
		if(j == -1)
		{
			printf("epoll error\n");
			return 0;
		}

		//deq
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		ioctl(fd, VIDIOC_DQBUF, &buf);

		//do
		//printmemory(info[cur].buf+0xfff, 16);
		eventwrite(cur, 'v', 0, 0);

		//enq
		ioctl(fd, VIDIOC_QBUF, &buf);
		cur = (cur+1)%24;
	}

	//stop
	j = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(ioctl(fd, VIDIOC_STREAMOFF, &j) == -1)
	{
		printf("error@OFF\n");
	}
	for(j=0;j<24;j++)
	{
		munmap(info[j].buf, info[j].len);
	}

	//v4l2_release
	close(fd);
	return 0;
}




int videolist()
{
	return 0;
}
int videochoose()
{
	return 0;
}
void* videoread(char* buf, int frame)
{
	return &info[(cur+23)%24];
	return 0;
}
int videowrite(char* buf, int frame)
{
	return 0;
}
int videostop()
{
	alive = 0;
	return 0;
}
int videostart(struct arena* win)
{
	alive = 1;
	startthread(visionlistener, 0);
	return 1;
}
void videodelete()
{
}
void videocreate()
{
}




void initcam()
{
}
void freecam()
{
}
